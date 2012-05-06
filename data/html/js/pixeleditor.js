var ajaxPrefix = 'http://localhost:9090/'; // For development
// var ajaxPrefix = '/'; // For deployment

var pe = new PixelEditor();

function PixelEditor()
{
	var _stage;
	var _layoutLayer;
	var _actorLayer;

	var _record;
	var _recordId;
	var _schema;
	var _entities;

	var _updateCollisionTimer;

	$(document).ready(onLoad);

	function onLoad()
	{
		populateNavBar();
		populateButtons();
		updateSchema();
		initStage();
		initUi();

		$(window).resize(function() {
			_stage.setSize($('#canvas').width(), $('#canvas').height());
		});
	}

	function populateNavBar()
	{
		$("#navbar").empty();
		$('<li class="nav-header">Records</li>').appendTo("#navbar");
		$.getJSON(ajaxPrefix + 'records/', function(data) {
			for (recordIdx in data)
			{
				var record = data[recordIdx];
				var recordItem = $("<li data-id='"+record.id+"'><a href='#'>"+record.name+"</a></li>").
					click(function() {loadRecord($(this).attr("data-id"));}).
					appendTo('#navbar');
			}
		});
	}

	function populateButtons()
	{
		$("#save").click(onSave);
		$("#export").click(onExport);
		$("#deselect").click(onDeselect);
	}

	function updateSchema()
	{
		$.getJSON(ajaxPrefix + 'schema/', function(data) {
			schema = new Object();

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
					$("<li data-id='"+schema[item].name+"'><a href='#'>"+description+"</a></li>").click(function() {createEntity($(this).attr("data-id"));}).appendTo('#entityTypes');
				}
				if (schema[item].type == "record")
				{
					$("<li data-id='"+schema[item].name+"'><a href='#'>"+description+"</a></li>").click(function() {
						$('#createRecordModal').attr("data-id", $(this).attr("data-id"));
						$('#createRecordModal').modal();
					}).appendTo('#recordTypes');
				}
			}

			_schema = schema;

			$('.dropdown-toggle').dropdown();
		});
	}

	function initStage()
	{
		$('#canvas').empty();
		_stage = new Kinetic.Stage({container: 'canvas',
			width:$('#canvas').width(),
			height:$('#canvas').height()});

		$('#canvas').off('mousewheel');
		$('#canvas').on('mousewheel', function(event, delta, deltaX, deltaY) {
			event.preventDefault();

			_stage.setX(Math.round(_stage.getX() - deltaX*100));
			_stage.setY(Math.round(_stage.getY() + deltaY*100));
			
			if (_updateCollisionTimer != null)
				window.clearTimeout(_updateCollisionTimer);
			
			_updateCollisionTimer = window.setTimeout(function() {
				updateCollisionData(false);
			}, 100);

			_stage.draw();
		});

		_layoutLayer = new Kinetic.Layer();
		_actorLayer = new Kinetic.Layer();
		_stage.add(_layoutLayer);
		_stage.add(_actorLayer);
	}

	function updateCollisionData(clear)
	{
		for (entityId in _entities)
		{
			var entity = _entities[entityId];
			if (clear)
				entity.shape.clearData();
			else
				entity.shape.saveData();
		}
		
	}

	function initUi()
	{
		$('#createRecordModal').submit(function() {
			$('#createRecordModal').modal('hide');

			$.ajax({
				type: 'POST',
				url: ajaxPrefix+'records?type='+$(this).attr("data-id")+'&name='+$('#createRecordName').attr("value"),
				complete : function() {
					populateNavBar();
				}
			});

			return false;
		});
	}

	function loadRecord(recordId)
	{
		initStage();

		_recordId = recordId;
		_entities = new Object();

		$.getJSON(ajaxPrefix + 'record/' + recordId, function(data) {
			_record = data;
			initialiseRecord(_record);
			for (entityIdx in _record.Entities)
			{
				var entity = _record.Entities[entityIdx];
				_entities[entity.Uid] = entity;
				initialiseEntity(entity);
			}
			_record.Entities = null;
		});
	}

	function createRecord(recordType)
	{
		$.ajax({
			type: 'POST',
			url: ajaxPrefix+'records?type='+recordType,
			complete : function() {
				populateNavBar();
			}
		});
	}

	function createEntity(entityType)
	{
		$.ajax({
			type: 'POST',
			url: ajaxPrefix+'record/'+_recordId+'/entities?type='+entityType,
			complete : function() {
				loadRecord(_recordId);
			}
		});
	}

	function initialiseRecord(record)
	{
		var width = 50;
		var height = 20;

		var shape = new Kinetic.Rect({
			x: -toPixels(width/2),
			y: -toPixels(height/2),
			width: toPixels(width),
			height: toPixels(height),
			fill: 'lightgrey',
			stroke: 'black',
			strokeWidth: 1,
			draggable: false
		});

		shape.on("mousedown", function(evt) {
			updateCollisionData(true);
			generateStructProperties(_record);
			this.dragStart = {x: evt.clientX, y: evt.clientY};

			this.on("mousemove", function(evt) {
				var xDiff = this.dragStart.x - evt.clientX;
				var yDiff = this.dragStart.y - evt.clientY;
				this.dragStart.x = evt.clientX;
				this.dragStart.y = evt.clientY;
				_stage.setX(Math.round(_stage.getX() - xDiff));
				_stage.setY(Math.round(_stage.getY() - yDiff));
				_stage.draw();
			});
		});

		shape.on("mouseup", function(evt) {
			updateCollisionData(false);
			this.off("mousemove");
		});

		_layoutLayer.add(shape);

		var x;
		for (x = Math.ceil(-width/2); x <= width/2; x++)
		{
			var points = [{x: toPixels(x), y: -toPixels(height/2)}, {x: toPixels(x), y: toPixels(height/2)}];
			shape = new Kinetic.Line({
				points: points,
				stroke: 'grey',
				strokeWidth: 1
			});

			_layoutLayer.add(shape);
		}

		var y;
		for (y = Math.ceil(-height/2); y <= height/2; y++)
		{
			var points = [{x: -toPixels(width/2), y: toPixels(y)}, {x: toPixels(width/2), y: toPixels(y)}];
			shape = new Kinetic.Line({
				points: points,
				stroke: 'grey',
				strokeWidth: 1
			});

			_layoutLayer.add(shape);
		}

		_layoutLayer.draw();
		_stage.draw();
	}

	function initialiseEntity(entity)
	{
		var schemaStruct = schema[entity.Type];
		var shape;

		if (!schemaStruct)
		{
			shape = new Kinetic.Circle({
				x: toPixels(entity.Transform.tx),
				y: toPixels(entity.Transform.ty),
				radius: 32,
				fill: 'grey',
				stroke: 'black',
				strokeWidth: 1.5,
				draggable: true
			});
		} else {
			var visualisation = schemaStruct.attributes.Visualisation;
			switch (visualisation.type)
			{
				case "sprite":
				{
					var sprite = new Image();
					sprite.src = ajaxPrefix+'images/'+visualisation.sprite+'.png';
					sprite.onload = function() {
						shape = new Kinetic.Image({
							image: sprite,
							x: toPixels(entity.Transform.tx),
							y: toPixels(entity.Transform.ty),
							centerOffset: {x: sprite.width/2, y: sprite.height/2},
							width: sprite.width,
							height: sprite.height,
							draggable: true,
							detectionType: 'pixel'
						});

						setupShape(shape, entity);
					}
					return;
				}
				default:
				{
					shape = new Kinetic.Circle({
						x: toPixels(entity.Transform.tx),
						y: toPixels(entity.Transform.ty),
						radius: 32,
						fill: 'red',
						stroke: 'black',
						strokeWidth: 1.5,
						draggable: true
					});
					break;
				}
			}
		}

		setupShape(shape, entity);
	}

	function setupShape(shape, entity)
	{
		entity.shape = shape;

		shape.entityId = entity.Uid;

		shape.on("click", function(evt) {
			generateStructProperties(_entities[evt.shape.entityId]);
		});

		shape.on("dragend", function(evt) {
			var url = ajaxPrefix+'record/'+_recordId+'/entity/'+evt.shape.entityId+'/transform?tx='+getX(evt.shape)+'&ty='+getY(evt.shape);
			$.ajax({
				type: 'PUT',
				url: url,
				complete : function() {
				}
			});

			this.saveData();
		});

		_actorLayer.add(shape);

		_actorLayer.draw();

		shape.saveData();
	}

	function onSave()
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

	function onExport()
	{
		$.ajax({
			type: 'PUT',
			url: ajaxPrefix+'export',
			complete : function() {
			}
		});
	}

	function onDeselect()
	{
		generateStructProperties(_record);
	}

	function generateStructProperties(struct)
	{
		if (!struct)
			return;

		var parent = $("#propertyArea");
		parent.empty();
		generateProperties(parent, "/", schema[struct.Type], struct.Properties);
	}

	function generateProperties(parent, path, schemaItem, properties)
	{
		if (!schemaItem)
			return;

		for (propertyIdx in schemaItem.properties)
		{
			var property = schemaItem.properties[propertyIdx];
			addProperty(parent, path + property.name + "/", property, properties);
		}
	}

	function addProperty(parent, path, property, properties)
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
				generateProperties(child, path, schema[property.structType], properties ? properties[property.name] : null);
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
				child.appendTo(parent);
				break;
			}
		}
	}

	function toPixels(unit)
	{
		return unit*32;
	}

	function fromPixels(pixels)
	{
		return pixels/32;
	}

	function getX(shape)
	{
		return fromPixels(shape.getPosition().x);
	}

	function getY(shape)
	{
		return fromPixels(shape.getPosition().y);
	}
}
