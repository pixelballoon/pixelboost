function AxisProperty(entity)
{
	var entity;
	var xAxis;
	var yAxis;
	var center;
	var sprite;

	this.entity = entity;
	
	var points;
	var length = pb.toPixels(2);
	var centerSize = pb.toPixels(0.25);

	this.center = new Kinetic.Rect({
		x: -centerSize/2,
		y: -centerSize/2,
		width: centerSize,
		height: centerSize,
		fill: 'yellow',
		stroke: 'grey',
		strokeWidth: 0.5
	});

	points = [{x: 0, y: 0}, {x: length, y: 0}];
	this.xAxis = new Kinetic.Line({
		points: points,
		stroke: 'red',
		strokeWidth: 1
	});

	points = [{x: 0, y: 0}, {x: 0, y: -length}];
	this.yAxis = new Kinetic.Line({
		points: points,
		stroke: 'green',
		strokeWidth: 1
	});

	this.entity.addShape(this.xAxis);
	this.entity.addShape(this.yAxis);
	this.entity.addShape(this.center);
}

AxisProperty.prototype.refresh = function()
{

}

AxisProperty.prototype.updateCollisionData = function()
{

}