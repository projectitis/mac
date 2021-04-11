(function(){Math.clamp=function(a,b,c){return Math.max(b,Math.min(c,a));}})();
(function(){Math.clampi=function(a,b,c){return Math.max(Math.round(b),Math.min(Math.round(c),Math.round(a)));}})();
class Graphics {

	/**
	 * Set up the graphics object
	 */
	constructor() {
		this.canvas = document.querySelector( '#canvas' );
		this.ctx = this.canvas.getContext( '2d' );
		this.canvasRect = this.canvas.getBoundingClientRect();
		this.width = 320;
		this.height = 240;

		this.ctx.fillStyle = 'rgb(0, 0, 0)';
		this.ctx.fillRect( 0, 0, this.width, this.height );

		var gfx = this;

		let resetBtn = document.querySelector( '#reset' );
		if (resetBtn){
			resetBtn.addEventListener( "click", function(){ gfx.reset(); } );
		}

		let runBtn = document.querySelector( '#run' );
		if (runBtn){
			runBtn.addEventListener( "click", function(){ gfx.run(); } );
		}

		this.pixel = this.ctx.createImageData(1,1);

		this.inspector = document.querySelector( '#inspector' );
		this.canvas.addEventListener( "mouseover", function( evt ){ gfx.movein( evt ); } );
		this.canvas.addEventListener( "mouseout", function( evt ){ gfx.moveout( evt ); } );
		this.canvas.addEventListener( "mousemove", function( evt ){ gfx.move( evt ); } );
		this.canvas.addEventListener( "click", function( evt ){ gfx.movelog( evt ); } );
	}

	movein( evt ) {
		this.inspector.style.visibility = 'visible';
	}
	moveout( evt ) {
		this.inspector.style.visibility = 'hidden';
	}
	move( evt ) {
		let x = Math.floor((evt.pageX - this.canvasRect.left) / 4 );
		let y = Math.floor((evt.pageY - this.canvasRect.top) / 4 );
		let c = this.getPixel( x, y );
		this.inspector.querySelector('#i_x').textContent = x;
		this.inspector.querySelector('#i_y').textContent = y;
		this.inspector.querySelector('#i_cr').textContent = c.r;
		this.inspector.querySelector('#i_cg').textContent = c.g;
		this.inspector.querySelector('#i_cb').textContent = c.b;
		this.inspector.style.top = evt.pageY+'px';
		this.inspector.style.left = (evt.pageX + 10) + 'px';
		this.inspector.querySelector('#swatch').style.backgroundColor = this.colorToHex( c )
	}
	movelog( evt ) {
		let x = Math.floor((evt.pageX - this.canvasRect.left) / 4 );
		let y = Math.floor((evt.pageY - this.canvasRect.top) / 4 );
		let c = this.getPixel( x, y );
		console.log({
			x: x, y: y, r:c.r, g:c.r, b:c.r, c: this.colorToHex( c )
		});
	}

	/**
	 * Reset the canvas
	 */
	reset() {
		this.ctx.fillStyle = 'rgb(0, 0, 0)';
		this.ctx.fillRect( 0, 0, this.width, this.height );
		console.log("\nRESET\n");
	}

	/**
	 * Run the user code (main)
	 */
	run () {
		console.log("\nRUN\n");
		let main = new Main( this );
		main.main();
	}

	/**
	 * Return a color object with RGB and A components
	 * @param {*} r 	Int, 0-255
	 * @param {*} g 	Int, 0-255
	 * @param {*} b 	Int, 0-255
	 * @param {*} a 	Float, 0-1
	 * @returns 	Object {r,g,b,a}
	 */
	color( r, g, b, a=1 ) {
		return {
			r: Math.clampi( r, 0, 255 ),
			g: Math.clampi( g, 0, 255 ),
			b: Math.clampi( b, 0, 255 ),
			a: Math.clamp( a, 0, 1 )
		}
	}

	colorToHex( c ) {
		return "#" + ((1 << 24) + (c.r << 16) + (c.g << 8) + c.b).toString(16).slice(1);
	}

	/**
	 * Draw a single pixel to the canvas
	 * @param {*} x 		Int, X coord
	 * @param {*} y 		Int, Y coord
	 * @param {*} color 	Color object [r,g,b,a}]
	 * @returns 	True if successfull, false if pixel out of bounds
	 */
	setPixel( x, y, color ) {
		if ( (x<0) || (y<0) || (x>=this.width) || (y>=this.height) ){
			console.log('ERROR: putPixel out of bounds', {x:x, y:y})
			return false;
		}
		if (!color.a && !(color.a === 0)) color.a = 1;
		this.pixel.data[0] = Math.clampi( color.r, 0, 255 );
		this.pixel.data[1] = Math.clampi( color.g, 0, 255 );
		this.pixel.data[2] = Math.clampi( color.b, 0, 255 );
		this.pixel.data[3] = Math.round(Math.clamp( color.a, 0, 1 ) * 255);

		this.ctx.putImageData( this.pixel, x, y );

		return true;
	}

	/**
	 * Get a pixel and return it as a color object {r,g,b,a}. If the pixel is
	 * out of bounds this will be wrtitten to teh console as an error, but a
	 * black transparent pixel will be returned {r:0,g:0,b:0,a:0}
	 * @param {*} x 
	 * @param {*} y 
	 * @returns 	color object for pixel {r,g,b,a}
	 */
	getPixel( x, y ) {
		if ( (x<0) || (y<0) || (x>=this.width) || (y>=this.height) ){
			console.log('ERROR: getPixel out of bounds', {x:x, y:y})
			return { r:0, g:0, b:0, a:0 };
		}
		let pix = this.ctx.getImageData( x, y, 1, 1 );
		return {
			r: pix.data[0],
			g: pix.data[1],
			b: pix.data[2],
			a: pix.data[0]/255
		};
	}
}

/**
 * Instantiate the graphics ibject
 */
new Graphics();
