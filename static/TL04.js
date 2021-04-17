class TL04 {
    constructor(left, right) {
        // Ids de los botones usados en versión móvil
        this.left = left;
        this.right = right;

        // Variable para evitar pulsaciones repetidas
        this.lastPress = performance.now();

        // Enganchar las funciones encargadas de procesar pulsaciones del teclado y/o botones
        document.onkeypress = this.onkeypress.bind(this);
        document.addEventListener('click', this.onclick.bind(this));
    }

    checkLastPress() {
        // Si la última pulsación ha ocurrido hace menos de 200ms devolver false
        if ((performance.now() - this.lastPress) < 200) return false;
        this.lastPress = performance.now();
        return true;
    }

    toggle(id) {
        // Cosas de bootstrap...
        var button = document.getElementById(id);
        var bsButton = new bootstrap.Button(button);
        bsButton.toggle();
    }

    press(key) {
        // Mandar la tecla pulsada a la Raspberry
        fetch('/teclado?tecla=' + key);
    }

    onkeypress(e) {
        // Si se ha pulsado hace poco no hacer nada
        if (!this.checkLastPress()) return;

        // Procesar la tecla pulsada y enviar los datos
        e = e || window.event;
        if (e.keyCode == 97) {
            this.press('a');
            this.toggle(this.left);
            setTimeout(function () {
                this.toggle(this.left);
            }.bind(this), 100);
        } else if (e.keyCode == 100) {
            this.press('d');
            this.toggle(this.right);
            setTimeout(function () {
                this.toggle(this.right);
            }.bind(this), 100);
        } else {
            this.press('s');
        }
    }

    onclick(elm) {
        // Si se ha pulsado hace poco no hacer nada
        if (!this.checkLastPress()) return;

        // Procesar botón pulsado
        var key = elm.target.id;
        if (['a', 'd'].indexOf(key) == -1) return;
        this.press(key);
    }
}