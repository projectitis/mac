class Main {

	lines

	/**
	 * Constructor. Called by graphics class automatically.
	 * @param {*} gfx 	The graphics object to draw with
	 */
	constructor( gfx ) {
		this.gfx = gfx;
	}

	/**
	 * All user code here
	 */
	main() {

		// Add a polygon to draw
		this.addPoly( 0xff0000, 0.5, [
			20,20,
			200,80,
			60,220
		 ] );

		/*
		let c = this.gfx.color(255,255,255);
		for (let i=20; i<100; i++){

			c.r = Math.round( Math.random()*255 );
			c.g = Math.round( Math.random()*255 );
			c.b = Math.round( Math.random()*255 );

			this.gfx.setPixel( i, i, c );
		}
		*/

	}

	addPoly( color, alpha, path ) {
		// XXX: Check path contains at least 3 points
		// XXX: Check even number of elems in path

		// Auto close
		if ( (path[0] != path[path.length-2]) || (path[1] != path[path.length-1])) {
			path.push( path[0], path[1] );
		}

		// Draw to canvas
		this.gfx.ctx.beginPath();
		this.gfx.ctx.strokeStyle = 'rgba(0,0,0,0)';
		this.gfx.ctx.fillStyle = 'rgba(255,100,0,0.5)';
		
		// Step points
		var x1 = path.shift();
		var y1 = path.shift();
		this.gfx.ctx.moveTo(x1, y1);
		var x2, y2 = 0;
		while (path.length) {
			x2 = path.shift();
			y2 = path.shift();

			this.gfx.ctx.lineTo(x2, y2);

			x1 = x2; y1 = y2;
		}

		this.gfx.ctx.stroke();
	}

}
