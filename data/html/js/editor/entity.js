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

	this.createProperties("/", schemaStruct);
}

Entity.prototype.createProperties = function(path, schemaStruct)
{
	if (!schemaStruct)
		return;

	this.createProperty(path, schemaStruct.attributes);

	for (propertyIdx in schemaStruct.properties)
	{
		var property = schemaStruct.properties[propertyIdx];
		
		var child = path + property.name + "/";

		this.createProperty(child, property.attributes);

		if (property.type == "struct")
		{
			var schemaStruct = pb.schema[property.structType];
			this.createProperties(child, schemaStruct);
		}
	}

	this.addProperty(new AxisProperty(this));
}

Entity.prototype.createProperty = function(path, attributes)
{
	var visualisation = attributes.Visualisation;

	if (visualisation)
	{
		switch (visualisation.type)
		{
			case "sprite":
			{
				this.addProperty(new SpriteProperty(this, visualisation, path));
				break;
			}
		}
	}
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

Entity.prototype.removeShape = function(shape)
{
	this.group.remove(shape);
	this.layer.draw();
}

Entity.prototype.evaluateProperty = function(path, value, callback)
{
	if (value.charAt(0) == "/")
	{
		var url = ajaxPrefix + 'record/' + this.recordId + "/entity/" + this.data.Uid + path + value.substring(1);
		$.getJSON(url, _.bind(function(data) {
			if (data)
				callback(data.value);
		}));
	} else {
		callback(value);
	}
}

Entity.prototype.refreshProperties = function()
{
	for (propertyIdx in this.properties)
	{
		var property = this.properties[propertyIdx];
		property.refresh();
	}
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
		pb.generateStructProperties("record/"+this.recordId+"/entity/"+this.data.Uid+"/", this.data);
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
