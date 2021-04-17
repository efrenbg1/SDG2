
var display = new LedDisplay('pantalla');
var tl04 = new TL04('a', 'd');

// Para no saturar la raspi se permite solo una petición de refresco a la vez
var displayLock = false;
setInterval(function () {

    // No actualizar pantalla si la petición anterior no ha terminado
    if (displayLock) return;
    displayLock = true;

    fetch('/pantalla').then(response => {

        // Si se produce algún error avisar al usuario y parar el refresco de la pantalla
        if (response.status != 200) {
            document.getElementById('error').style.display = 'flex';
            return;
        }

        // Actualizar pantalla con los nuevos datos
        displayLock = false;
        response.json().then(function (obj) {
            for (var y = 0; y < obj.length; y++) {
                for (var x = 0; x < obj[y].length; x++) {
                    display.led(x, y, obj[y][x]);
                }
            }
        });
    });
}, 200);

