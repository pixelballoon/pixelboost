var ajaxPrefix = 'http://localhost:9090/'; // For development
// var ajaxPrefix = '/'; // For deployment

require(["editor/entity", "editor/record"]);

function PixelEditor()
{
	var stage;
	var layoutLayer;
	var actorLayer;

	var record;
	var schema;	

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

	this.stage.setScale(0.5);
}

PixelEditor.prototype.addLayer = function(layer)
{
	this.stage.add(layer);
}

PixelEditor.prototype.updateCollisionData = function(clear)
{
	this.record.updateCollisionData(clear);
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
	this.record = new Record(recordId);
	this.record.load();
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
		url: ajaxPrefix+'record/'+this.record.data.Uid+'/entities?type='+entityType,
		complete : _.bind(function() {
			this.record.load();
		}, this)
	});
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

PixelEditor.prototype.generateStructProperties = function(root, struct)
{
	if (!struct)
		return;

	var parent = $("#propertyArea");
	parent.empty();
	this.generateProperties(root, struct.Uid, parent, "/", this.schema[struct.Type], struct.Properties);
}

PixelEditor.prototype.generateProperties = function(root, uid, parent, path, schemaItem, properties)
{
	if (!schemaItem)
		return;

	for (propertyIdx in schemaItem.properties)
	{
		var property = schemaItem.properties[propertyIdx];
		this.addProperty(root, uid, parent, path + property.name + "/", property, properties);
	}
}

PixelEditor.prototype.addProperty = function(root, uid, parent, path, property, properties)
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
			this.generateProperties(root, uid, child, path, this.schema[property.structType], properties ? properties[property.name] : null);
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
				var url = ajaxPrefix+root.substring(1)+"property"+path+"?type=atom&value="+value;
				$.ajax({
					type: 'PUT',
					url: url,
					complete : function() {
					}
				});
				this.properties[this.property.name] = value;
				pb.refreshEntity(uid);
			}, {root: root, path:path, properties:properties, property:property}));
			child.appendTo(parent);
			break;
		}
	}
}

PixelEditor.prototype.refreshEntity = function(entityId)
{
	var entity = pb.record.entities[entityId];
	if (entity)
		entity.refreshProperties();
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
