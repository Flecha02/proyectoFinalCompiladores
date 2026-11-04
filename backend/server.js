const express = require('express');
const cors = require('cors');
const fs = require('fs');
const { exec } = require('child_process');
const path = require('path');

const app = express();
const PORT = 5000;

app.use(cors());
app.use(express.json());

app.post('/api/compile', (req, res) => {
    const { codigo } = req.body;

    if (!codigo) {
        return res.status(400).json({ error: 'No se recibió código fuente.' });
    }

    const raizProyecto = path.join(__dirname, '..');
    const archivoEntrada = path.join(raizProyecto, 'temp_codigo.el');
    
    // 1. Guardar el código fuente en la raíz
    fs.writeFileSync(archivoEntrada, codigo, 'utf8');

    // 2. Comando completo: metacompila (Flex/Bison), compila con GCC y ejecuta el binario 'final'
    const comando = `flex proyectoFinal.l && bison -d proyectoFinal.y && gcc lex.yy.c proyectoFinal.tab.c -o final && ./final < temp_codigo.el`;

    // 3. Ejecutar todo en cascada posicionado en la raíz del proyecto
    exec(comando, { cwd: raizProyecto }, (error, stdout, stderr) => {
        let codigoTac = '';
        let codigoAsm = '';

        const rutaTac = path.join(raizProyecto, 'codigo.tac');
        const rutaAsm = path.join(raizProyecto, 'codigo.asm');

        try {
            if (fs.existsSync(rutaTac)) {
                codigoTac = fs.readFileSync(rutaTac, 'utf8');
            }
            if (fs.existsSync(rutaAsm)) {
                codigoAsm = fs.readFileSync(rutaAsm, 'utf8');
            }
        } catch (err) {
            console.error('Error al leer archivos de salida:', err);
        }

        if (error) {
            return res.json({
                exito: false,
                mensaje: stderr || error.message,
                tac: codigoTac,
                asm: codigoAsm
            });
        }

        res.json({
            exito: true,
            mensaje: stderr || '¡Metacompilación y ejecución exitosa!',
            tac: codigoTac,
            asm: codigoAsm
        });
    });
});

app.listen(PORT, () => {
    console.log(`Servidor backend corriendo en http://localhost:${PORT}`);
});