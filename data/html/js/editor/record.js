function Record(recordId)
{
	var uid;
	var data;
	var entities;
	var background;

	this.background = {};
	this.uid = recordId;
}

Record.prototype.load = function()
{
	this.entities = new Object();

	$.getJSON(ajaxPrefix + 'record/' + this.uid, _.bind(function(data) {
		this.data = data;
		this.initialise();
		for (entityIdx in this.data.Entities)
		{
			var entity = this.data.Entities[entityIdx];
			this.entities[entity.Uid] = new Entity(this.uid, pb.actorLayer, entity);
		}
		this.data.Entities = null;
	}, this));
}

Record.prototype.initialise = function()
{
	var schemaType = pb.schema[this.data.Type];
	
	if (!schemaType)
		return;

	if (schemaType.attributes.HasLevel)
	{
		var width = schemaType.attributes.HasLevel.width;
		var height = schemaType.attributes.HasLevel.height;

		this.evaluateProperty(width, _.bind(function(x) {
			this.evaluateProperty(height, _.bind(function(y) {
				this.createBackground(x, y);
			}, this))
		}, this));
	}
}

Record.prototype.createBackground = function(width, height)
{
	if (width == 0)
		width = 10;

	if (height == 0)
		height = 10;

	var background = new Kinetic.Rect({
		x: -pb.toPixels(width/2),
		y: -pb.toPixels(height/2),
		width: pb.toPixels(width),
		height: pb.toPixels(height),
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

	pb.layoutLayer.add(background);

	var shape;

	var x;
	for (x = Math.ceil(-width/2); x <= width/2; x++)
	{
		var points = [{x: pb.toPixels(x), y: -pb.toPixels(height/2)}, {x: pb.toPixels(x), y: pb.toPixels(height/2)}];
		shape = new Kinetic.Line({
			points: points,
			stroke: 'grey',
			strokeWidth: 1
		});

		pb.layoutLayer.add(shape);
	}

	var y;
	for (y = Math.ceil(-height/2); y <= height/2; y++)
	{
		var points = [{x: -pb.toPixels(width/2), y: pb.toPixels(y)}, {x: pb.toPixels(width/2), y: pb.toPixels(y)}];
		shape = new Kinetic.Line({
			points: points,
			stroke: 'grey',
			strokeWidth: 1
		});

		pb.layoutLayer.add(shape);
	}

	pb.stage.setX(pb.stage.getWidth()/2);
	pb.stage.setY(pb.stage.getHeight()/2);
	pb.stage.draw();
}

Record.prototype.onDeselect = function()
{
	pb.generateStructProperties("/record/"+this.data.Uid+"/", this.data);
}

Record.prototype.updateCollisionData = function(clear)
{
	for (entityId in this.entities)
	{
		var entity = this.entities[entityId];

		entity.updateCollisionData(clear);
	}
}

Record.prototype.evaluateProperty = function(value, callback)
{
	if (value.charAt(0) == "/")
	{
		var url = ajaxPrefix + 'record/' + this.data.Uid + "/property" + value;
		$.getJSON(url, _.bind(function(data) {
			if (data)
				callback(data.value);
			else
				callback("");
		}));
	} else {
		callback(value);
	}
}
