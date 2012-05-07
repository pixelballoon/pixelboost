function SpriteProperty(entity, visualisation)
{
	var entity;
	var shape;
	var sprite;

	this.entity = entity;
	this.sprite = new Image();

	this.sprite.src = ajaxPrefix+'images/'+visualisation.sprite+'.png';
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
}

SpriteProperty.prototype.updateCollisionData = function(clear)
{
	if (clear)
		this.shape.clearData();
	else
		this.shape.saveData();
}
