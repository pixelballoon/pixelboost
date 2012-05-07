var ajaxPrefix = 'http://localhost:9090/'; // For development
// var ajaxPrefix = '/'; // For deployment

require(["editor/entity"]);

function PixelEditor()
{
	var stage;
	var layoutLayer;
	var actorLayer;

	var record;
	var recordId;
	var schema;
	var entities;

	$(document).ready(_.bind(function() {this.onLoad();}, this));
}

PixelEditor.prototype.onLoad = function(a, b, c)
{
	this.populateNavBar();
	this.populateButtons();
	this.updateSchema();
	this.initStage();
	this.initUi();

	$(window).resize(function() {
		stage.setSize($('#canvas').width(), $('#canvas').height());
	});
};	

PixelEditor.prototype.populateNavBar = function()
{
	$("#navbar").empty();
	$('<li class="nav-header">Records</li>').appendTo("#navbar");
	$.getJSON(ajaxPrefix + 'records/', _.bind(function(data) {
		for (recordIdx in data)
		{
			var record = data[recordIdx];
			var recordItem = $("<li data-id='"+record.id+"'><a href='#'>"+record.name+"</a></li>").
				click(function() {
					pb.loadRecord($(this).attr("data-id"));
				}).appendTo('#navbar');
		}
	}));
}

PixelEditor.prototype.populateButtons = function()
{
	$("#save").click(pb.onSave);
	$("#export").click(pb.onExport);
	$("#deselect").click(pb.onDeselect);
}

PixelEditor.prototype.updateSchema = function()
{
	$.getJSON(ajaxPrefix + 'schema/', function(data) {
		var schema = new Object();

		for (schemaItem in data)
		{
			schema[data[schemaItem].name] = data[schemaItem];
		}

		for (item in schema)
		{
			var description = null;

			if (schema[item].attributes.Description != null)
				description = schema[item].attributes.Description.name;

			if (description == null)
				description = schema[item].name;

			if (schema[item].type == "entity")
			{
				$("<li data-id='"+schema[item].name+"'><a href='#'>"+description+"</a></li>").click(function() {pb.createEntity($(this).attr("data-id"));}).appendTo('#entityTypes');
			}
			if (schema[item].type == "record")
			{
				$("<li data-id='"+schema[item].name+"'><a href='#'>"+description+"</a></li>").click(function() {
					$('#createRecordModal').attr("data-id", $(this).attr("data-id"));
					$('#createRecordModal').modal();
				}).appendTo('#recordTypes');
			}
		}

		pb.schema = schema;

		$('.dropdown-toggle').dropdown();
	});
}

PixelEditor.prototype.initStage = function()
{
	$('#canvas').empty();
	this.stage = new Kinetic.Stage({container: 'canvas',
		width:$('#canvas').width(),
		height:$('#canvas').height()});

	var lazyUpdateCollision = _.debounce(_.bind(function() { this.updateCollisionData(false)}, this), 300);

	$('#canvas').off('mousewheel');
	$('#canvas').on('mousewheel', _.bind(function(event, delta, deltaX, deltaY) {
		event.preventDefault();

		this.stage.setX(Math.round(pb.stage.getX() - deltaX*100));
		this.stage.setY(Math.round(pb.stage.getY() + deltaY*100));

		lazyUpdateCollision();

		this.stage.draw();
	}, this));

	this.layoutLayer = new Kinetic.Layer();
	this.actorLayer = new Kinetic.Layer();
	this.addLayer(this.layoutLayer);
	this.addLayer(this.actorLayer);

	this.stage.setScale(1);
}

PixelEditor.prototype.addLayer = function(layer)
{
	this.stage.add(layer);
}

PixelEditor.prototype.updateCollisionData = function(clear)
{
	for (entityId in this.entities)
	{
		var entity = this.entities[entityId];

		entity.updateCollisionData(clear);
	}
}

PixelEditor.prototype.initUi = function()
{
	$('#createRecordModal').submit(function() {
		$('#createRecordModal').modal('hide');

		$.ajax({
			type: 'POST',
			url: ajaxPrefix+'records?type='+$(this).attr("data-id")+'&name='+$('#createRecordName').attr("value"),
			complete : function() {
				pb.populateNavBar();
			}
		});

		return false;
	});
}

PixelEditor.prototype.loadRecord = function(recordId)
{
	this.initStage();

	this.recordId = recordId;
	this.entities = new Object();

	$.getJSON(ajaxPrefix + 'record/' + recordId, _.bind(function(data) {
		this.record = data;
		this.initialiseRecord(this.record);
		for (entityIdx in this.record.Entities)
		{
			var entity = this.record.Entities[entityIdx];
			this.entities[entity.Uid] = new Entity(pb.recordId, pb.actorLayer, entity);
		}
		this.record.Entities = null;
	}, this));
}

PixelEditor.prototype.createRecord = function(recordType)
{
	$.ajax({
		type: 'POST',
		url: ajaxPrefix+'records?type='+recordType,
		complete : function() {
			populateNavBar();
		}
	});
}

PixelEditor.prototype.createEntity = function(entityType)
{
	$.ajax({
		type: 'POST',
		url: ajaxPrefix+'record/'+this.recordId+'/entities?type='+entityType,
		complete : _.bind(function() {
			this.loadRecord(this.recordId);
		}, this)
	});
}

PixelEditor.prototype.initialiseRecord = function(record)
{
	var width = 30;
	var height = 20;

	var background = new Kinetic.Rect({
		x: -this.toPixels(width/2),
		y: -this.toPixels(height/2),
		width: this.toPixels(width),
		height: this.toPixels(height),
		fill: 'lightgrey',
		stroke: 'black',
		strokeWidth: 1,
		draggable: false
	});

	background.on("mousedown", _.bind(function(evt) {
		this.updateCollisionData(true);
		this.onDeselect();

		background.dragStart = {x: evt.clientX, y: evt.clientY};

		background.on("mousemove", _.bind(function(evt) {
			var xDiff = background.dragStart.x - evt.clientX;
			var yDiff = background.dragStart.y - evt.clientY;
			background.dragStart.x = evt.clientX;
			background.dragStart.y = evt.clientY;
			this.stage.setX(Math.round(this.stage.getX() - xDiff));
			this.stage.setY(Math.round(this.stage.getY() - yDiff));
			this.stage.draw();
		}, this));
	}, this));

	background.on("mouseup", _.bind(function(evt) {
		this.updateCollisionData(false);
		background.off("mousemove");
	}, this));

	this.layoutLayer.add(background);

	var shape;

	var x;
	for (x = Math.ceil(-width/2); x <= width/2; x++)
	{
		var points = [{x: this.toPixels(x), y: -this.toPixels(height/2)}, {x: this.toPixels(x), y: this.toPixels(height/2)}];
		shape = new Kinetic.Line({
			points: points,
			stroke: 'grey',
			strokeWidth: 1
		});

		this.layoutLayer.add(shape);
	}

	var y;
	for (y = Math.ceil(-height/2); y <= height/2; y++)
	{
		var points = [{x: -this.toPixels(width/2), y: this.toPixels(y)}, {x: this.toPixels(width/2), y: this.toPixels(y)}];
		shape = new Kinetic.Line({
			points: points,
			stroke: 'grey',
			strokeWidth: 1
		});

		this.layoutLayer.add(shape);
	}

	this.stage.setX(this.stage.getWidth()/2);
	this.stage.setY(this.stage.getHeight()/2);
	this.stage.draw();
}

PixelEditor.prototype.onSave = function()
{
	$.ajax({
		type: 'PUT',
		url: ajaxPrefix+'save',
		success : function() {
			noty({"text":"Save successful","theme":"noty_theme_twitter","layout":"topCenter","type":"success","animateOpen":{"height":"toggle"},"animateClose":{"height":"toggle"},"speed":500,"timeout":1000,"closeButton":false,"closeOnSelfClick":true,"closeOnSelfOver":false,"modal":false});
		},
		error : function() {
			noty({"text":"Save failed","theme":"noty_theme_twitter","layout":"topCenter","type":"error","animateOpen":{"height":"toggle"},"animateClose":{"height":"toggle"},"speed":500,"timeout":1000,"closeButton":false,"closeOnSelfClick":true,"closeOnSelfOver":false,"modal":false});
		}
	});
}

PixelEditor.prototype.onExport = function()
{
	$.ajax({
		type: 'PUT',
		url: ajaxPrefix+'export',
		complete : function() {
		}
	});
}

PixelEditor.prototype.onDeselect = function()
{
	this.generateStructProperties("", this.record);
}

PixelEditor.prototype.generateStructProperties = function(root, struct)
{
	if (!struct)
		return;

	var parent = $("#propertyArea");
	parent.empty();
	this.generateProperties(root, parent, "/", this.schema[struct.Type], struct.Properties);
}

PixelEditor.prototype.generateProperties = function(root, parent, path, schemaItem, properties)
{
	if (!schemaItem)
		return;

	for (propertyIdx in schemaItem.properties)
	{
		var property = schemaItem.properties[propertyIdx];
		this.addProperty(root, parent, path + property.name + "/", property, properties);
	}
}

PixelEditor.prototype.addProperty = function(root, parent, path, property, properties)
{
	switch (property.type)
	{
		case "array":
			parent.append('<div class="well">'+property.name+': The array type is not yet supported</p></div>');
			break;
		case "pointer":
			parent.append('<div class="well">'+property.name+': The pointer type is not yet supported</p></div>');
			break;
		case "struct":
			$('<label class="control-label">'+property.name+'</label>').appendTo(parent);
			var child = $('<div class="control-group well"></div>');
			child.appendTo(parent);
			this.generateProperties(root, child, path, this.schema[property.structType], properties ? properties[property.name] : null);
			break;
		case "atom":
		{
			var value = properties[property.name];
			if (value == undefined)
			{
				value = "";
			}

			var child;
			switch (property.atomType)
			{
				case "float":
				case "int":
					child = $('<label class="control-label">'+property.name+'</label><input type="text" class="span4" value="'+value+'"></input><br/>');
					break;
				case "string":
					child = $('<label class="control-label">'+property.name+'</label><input type="text" class="span6" value="'+value+'"></input><br/>');
					break;
			}
			child.change(_.bind(function(evt) {
				var value = evt.target.value;
				var url = ajaxPrefix+root+"property"+path+"?type=atom&value="+value;
				$.ajax({
					type: 'PUT',
					url: url,
					complete : function() {
					}
				});
				this.properties[this.property.name] = value;
			}, {root: root, path:path, properties:properties, property:property}));
			child.appendTo(parent);
			break;
		}
	}
}

PixelEditor.prototype.toPixels = function(unit)
{
	return unit*32;
}

PixelEditor.prototype.fromPixels = function(pixels)
{
	return pixels/32;
}

var pb = new PixelEditor();
