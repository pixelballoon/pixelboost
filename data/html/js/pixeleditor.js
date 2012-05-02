var ajaxPrefix = 'http://localhost:9090/'; // For development
// var ajaxPrefix = '/'; // For deployment

var stage;
var actorLayer;

var _record;
var _recordId;
var _schema;
var _entities;

function onLoad()
{
	populateNavBar();
	populateButtons();
	updateSchema();
	initStage();
	initUi();
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

		for (struct in data.structs)
		{
			schema[data.structs[struct].name] = data.structs[struct];
		}

		for (item in schema)
		{
			if (schema[item].type == "entity")
			{
				$("<li data-id='"+schema[item].name+"'><a href='#'>"+schema[item].description+"</a></li>").click(function() {createEntity($(this).attr("data-id"));}).appendTo('#entityTypes');
			}
			if (schema[item].type == "record")
			{
				$("<li data-id='"+schema[item].name+"'><a href='#'>"+schema[item].description+"</a></li>").click(function() {
					$('#createRecordModal').attr("data-id", $(this).attr("data-id"));
					$('#createRecordModal').modal();
				}).appendTo('#recordTypes');
			}
		}

		$('.dropdown-toggle').dropdown();
	});
}

function initStage()
{
	$('#canvas').empty();
	stage = new Kinetic.Stage({container: 'canvas',
		width:600,
		height:600});

	actorLayer = new Kinetic.Layer();
	stage.add(actorLayer);

	stage.on("mousedown", function(evt) {
		generateStructProperties(_record);
	});
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
		switch (schemaStruct.visualisation.type)
		{
			case "sprite":
			{
				var sprite = new Image();
				sprite.src = ajaxPrefix+'images/'+schemaStruct.visualisation.sprite+'.png';
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



	actorLayer.add(shape);

	actorLayer.draw();

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
