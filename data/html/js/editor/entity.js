require(["editor/properties/axis", "editor/properties/sprite"]);

function Entity(recordId, layer, entity)
{
	var recordId;
	var layer;
	var data;
	var group;
	var properties;

	this.recordId = recordId;
	this.layer = layer;
	this.data = entity;
	this.properties = [];
	this.setupGroup();

	this.initialiseEntity(entity);
}

Entity.prototype.initialiseEntity = function(entity)
{
	var schemaStruct = pb.schema[entity.Type];
	var visualisation = schemaStruct ? schemaStruct.attributes.Visualisation : null;

	if (visualisation)
	{
		switch (visualisation.type)
		{
			case "sprite":
			{
				this.addProperty(new SpriteProperty(this, visualisation));
				break;
			}
		}
	}

	this.addProperty(new AxisProperty(this));
}

Entity.prototype.addProperty = function(property)
{
	this.properties.push(property);
}

Entity.prototype.addShape = function(shape)
{
	this.group.add(shape);
	shape.saveData();
	this.layer.draw();
}

Entity.prototype.setupGroup = function()
{
	this.group = new Kinetic.Group({
		x: pb.toPixels(this.data.Transform.tx),
		y: pb.toPixels(this.data.Transform.ty),
		draggable: true
	});

	this.group.entityId = this.data.Uid;

	this.group.on("click", _.bind(function(evt) {
		pb.generateStructProperties(this.data);
	}, this));

	this.group.on("dragend", _.bind(function(evt) {
		var url = ajaxPrefix+'record/'+this.recordId+'/entity/'+this.data.Uid+'/transform?tx='+this.getX()+'&ty='+this.getY();
		$.ajax({
			type: 'PUT',
			url: url,
			complete : function() {
			}
		});

		this.updateCollisionData(false);
	}, this));

	this.layer.add(this.group);
}

Entity.prototype.updateCollisionData = function(clear)
{
	for (propertyIdx in this.properties)
	{
		var property = this.properties[propertyIdx];
		property.updateCollisionData(clear);
	}
}

Entity.prototype.getX = function()
{
	return pb.fromPixels(this.group.getPosition().x);
}

Entity.prototype.getY = function()
{
	return pb.fromPixels(this.group.getPosition().y);
}
