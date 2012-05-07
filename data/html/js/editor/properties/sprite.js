function SpriteProperty(entity, visualisation, path)
{
	var entity;
	var shape;
	var path;
	var visualisation;
	var sprite;

	this.visualisation = visualisation;
	this.entity = entity;
	this.path = path;
	this.sprite = new Image();

	this.refresh();	
}

SpriteProperty.prototype.refresh = function()
{
	if (this.shape)
		this.entity.removeShape(this.shape);

	this.entity.evaluateProperty(this.path, this.visualisation.sprite, _.bind(function(value) {
		this.sprite.src = ajaxPrefix+'images/'+value+'.png';
		this.sprite.onload = _.bind(function() {
			this.shape = new Kinetic.Image({
				image: this.sprite,
				x: 0,
				y: 0,
				centerOffset: {x: this.sprite.width/2, y: this.sprite.height/2},
				width: this.sprite.width,
				height: this.sprite.height,
				draggable: true,
				detectionType: 'pixel'
			});
			this.entity.addShape(this.shape);
		}, this);
	}, this));
}

SpriteProperty.prototype.updateCollisionData = function(clear)
{
	if (this.shape)
	{
		if (clear)
			this.shape.clearData();
		else
			this.shape.saveData();
	}
}
