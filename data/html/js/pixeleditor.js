var ajaxPrefix = 'http://localhost:9090/'; // For development
// var ajaxPrefix = '/'; // For deployment

var canvas;
var record;
var schema;

function onLoad()
{
	populateNavBar();
	populateButtons();
	updateSchema();
	initCanvas();
}

function populateNavBar()
{
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
}

function updateSchema()
{
	$.getJSON(ajaxPrefix + 'schema/', function(data) {
		schema = data;

		var structs = schema.structs;

		for (item in structs)
		{
			if (structs[item].type == "entity")
			{
				$("<li data-id='"+structs[item].name+"'><a href='#'>"+structs[item].description+"</a></li>").click(function() {createEntity($(this).attr("data-id"));}).appendTo('#records');
			}
		}

		$('.dropdown-toggle').dropdown();
	});
}

function initCanvas()
{
	$('#canvas').empty();
	canvas = Raphael("canvas", 600, 600);
	canvas.setViewBox(-300, -300, 600, 600);
}

function startDrag()
{
	this.ox = this.type == "circle" ? this.attr("cx") : this.attr("x");
	this.oy = this.type == "circle" ? this.attr("cy") : this.attr("y");
}

function onDrag(dx, dy, x, y)
{
	this.attr("x", this.ox + dx);
	this.attr("y", this.oy + dy);
	this.attr("cx", this.ox + dx);
	this.attr("cy", this.oy + dy);
}

function loadRecord(record)
{
	initCanvas();

	this.record = record;

	$.getJSON(ajaxPrefix + 'record/' + record + '/entities', function(data) {
		for (entity in data)
		{
			var image = canvas.image("img/glyphicons-halflings.png", 100, 100, 100, 100);
			image.attr("x", toPixels(data[entity].data.Transform.tx));
			image.attr("y", toPixels(data[entity].data.Transform.ty));
			image.drag(onDrag, startDrag);
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

function onSave()
{
	$.ajax({
          type: 'PUT',
          url: ajaxPrefix+'save/',
          complete : function() {
          }
        });
}

function toPixels(unit)
{
	return unit*32;
}
