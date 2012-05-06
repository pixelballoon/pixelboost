function Entity(recordId, layer, entity)
{
	var recordId;
	var layer;
	var data;
	var shape;

	this.recordId = recordId;
	this.layer = layer;
	this.data = entity;

	this.initialiseEntity(entity);
}

Entity.prototype.initialiseEntity = function(entity)
{
	var shape;

	var schemaStruct = pb.schema[entity.Type];

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
				sprite.onload = _.bind(function() {
					this.shape = new Kinetic.Image({
						image: sprite,
						x: pb.toPixels(entity.Transform.tx),
						y: pb.toPixels(entity.Transform.ty),
						centerOffset: {x: sprite.width/2, y: sprite.height/2},
						width: sprite.width,
						height: sprite.height,
						draggable: true,
						detectionType: 'pixel'
					});

					this.setupShape();
				}, this);
				return;
			}
			default:
			{
				this.shape = new Kinetic.Circle({
					x: pb.toPixels(entity.Transform.tx),
					y: pb.toPixels(entity.Transform.ty),
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

	this.setupShape();
}

Entity.prototype.setupShape = function()
{
	this.shape.entityId = this.data.Uid;

	this.shape.on("click", function(evt) {
		pb.generateStructProperties(pb.entities[evt.shape.entityId].data);
	});

	this.shape.on("dragend", _.bind(function(evt) {
		var url = ajaxPrefix+'record/'+this.recordId+'/entity/'+evt.shape.entityId+'/transform?tx='+this.getX()+'&ty='+this.getY();
		$.ajax({
			type: 'PUT',
			url: url,
			complete : function() {
			}
		});

		this.shape.saveData();
	}, this));

	this.layer.add(this.shape);

	this.layer.draw();

	this.shape.saveData();
}

Entity.prototype.getShape = function()
{
	return this.shape;
}

Entity.prototype.getX = function()
{
	return pb.fromPixels(this.shape.getPosition().x);
}

Entity.prototype.getY = function()
{
	return pb.fromPixels(this.shape.getPosition().y);
}
