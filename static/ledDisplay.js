class LedDisplay {
    constructor(id) {
        // Variables y constantes
        this.canvas = document.getElementById(id);
        this.ctx = this.canvas.getContext('2d');
        this.on = 'rgb(255, 0, 255)';
        this.off = 'rgb(10, 3, 160)';
        this.ledSize = 0;
        this.ledMargin = 0;

        // Poner todos los leds a apagados
        this.calculateSizes();

        // Recalcular las dimensiones del display cada vez que cambien el tamaño de la pantalla
        this.calculateSizesTimeout;
        window.onresize = function () {
            clearTimeout(this.calculateSizesTimeout);
            this.calculateSizesTimeout = setTimeout(this.calculateSizes.bind(this), 200);
        }.bind(this);
    }

    led(x, y, state) {
        // Calcular coordenas para el led
        var ycor = Math.ceil(this.ledMargin + 2 * this.ledMargin * y + this.ledSize * y);
        var xcor = Math.ceil(this.ledMargin + 2 * this.ledMargin * x + this.ledSize * x);

        // Dibujar led en base al estado
        this.ctx.fillStyle = state ? this.on : this.off;
        this.ctx.fillRect(xcor, ycor, this.ledSize, this.ledSize);
    }

    calculateSizes() {
        // Ajustar tamaño del canvas a las dimensiones de la pantalla
        this.canvas.width = Math.min(window.innerWidth, window.innerHeight / 2, 400);
        this.canvas.height = this.canvas.width;

        // Calcular tamaño de los leds
        this.ledSize = this.canvas.width * 4 / (8 * 5);
        this.ledMargin = this.canvas.width / (8 * 10);

        // Repintar leds como apagados para no dejar el canvas en blanco
        for (var y = 0; y < 8; y++) {
            for (var x = 0; x < 8; x++) {
                this.led(x, y, false);
            }
        }
    }
}