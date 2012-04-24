var ajaxPrefix = 'http://localhost:9090/'; // For development
// var ajaxPrefix = '/'; // For deployment

var stage;
var actorLayer;

var record;
var schema;

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
		for (record in data)
		{
			var recordItem = $("<li data-id='"+data[record].id+"'><a href='#'>"+data[record].name+"</a></li>").
				click(function() {loadRecord($(this).attr("data-id"));}).
				appendTo('#navbar');
		}
	});
}

function populateButtons()
{
	$("#save").click(onSave);
	$("#export").click(onExport);
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

function loadRecord(record)
{
	initStage();

	this.record = record;

	$.getJSON(ajaxPrefix + 'record/' + record + '/entities', function(data) {
		for (entity in data)
		{
			initialiseEntity(data[entity]);
		}

		actorLayer.draw();
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
		url: ajaxPrefix+'record/'+record+'/entities?type='+entityType,
		complete : function() {
			loadRecord(record);
		}
	});
}

function initialiseEntity(entity)
{
	var schemaStruct = schema[entity.data.Type];
	var shape;

	if (!schemaStruct)
	{
		shape = new Kinetic.Circle({
			x: toPixels(entity.data.Transform.tx),
			y: toPixels(entity.data.Transform.ty),
			radius: 32,
			fill: 'grey',
			stroke: 'black',
			strokeWidth: 1.5,
			draggable: true
		});
	} else {
		shape = new Kinetic.Circle({
			x: toPixels(entity.data.Transform.tx),
			y: toPixels(entity.data.Transform.ty),
			radius: 32,
			fill: 'red',
			stroke: 'black',
			strokeWidth: 1.5,
			draggable: true
		});
	}

	shape.entityId = entity.data.Uid;

	shape.on("dragend", function(evt) {
		var url = ajaxPrefix+'record/'+record+'/entity/'+evt.shape.entityId+'/transform?tx='+getX(evt.shape)+'&ty='+getY(evt.shape);
		$.ajax({
			type: 'PUT',
			url: url,
			complete : function() {
			}
		});
	});

	actorLayer.add(shape);
}

function onSave()
{
	$.ajax({
		type: 'PUT',
		url: ajaxPrefix+'save',
		complete : function() {
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
