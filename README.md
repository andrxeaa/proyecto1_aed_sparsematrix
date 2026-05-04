# Proyecto 1: Hoja de Cálculo (Listas Enlazadas Cruzadas)

Este proyecto implementa una hoja de cálculo usando una Matriz Dispersa basada en listas enlazadas cruzadas en C++ puro. Incluye una interfaz gráfica sencilla utilizando SFML.

## Requisitos Previos e Instalación de SFML

Para compilar este proyecto necesitas un compilador C++17 (g++) y la librería gráfica **SFML 2.6.x**.

### Instalación de SFML en Windows (Recomendado)
Para Windows (MinGW 64-bit GCC 13.1.0), puedes instalar SFML automáticamente usando PowerShell. Abre tu terminal en la carpeta del proyecto y ejecuta estos comandos:

```powershell
Invoke-WebRequest -Uri "https://www.sfml-dev.org/files/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit.zip" -OutFile "SFML.zip"
Expand-Archive -Path "SFML.zip" -DestinationPath "."
Rename-Item -Path "SFML-2.6.1" -NewName "SFML"
Remove-Item "SFML.zip"
```
Esto descargará y preparará la carpeta `SFML` exacta que el proyecto necesita.

### Instalación de SFML en Linux / Ubuntu / WSL
Es mucho más fácil. Abre tu terminal y ejecuta:
```bash
sudo apt update
sudo apt install libsfml-dev
```

## Compilación del Proyecto

Tienes dos opciones para compilar y ejecutar la hoja de cálculo:

### Opción A: Usando CLion (CMake) - ¡Más fácil!
1. Abre CLion y selecciona **Open**.
2. Selecciona el archivo `CMakeLists.txt` que está en esta carpeta y elige "Open as Project".
3. CLion configurará automáticamente todo gracias al archivo CMake.
4. Presiona el botón verde de **Play** (Run) en la esquina superior derecha.

### Opción B: Usando la Terminal (Makefile)
1. Abre tu terminal (en Windows asegúrate de usar MSYS2 o tener `mingw32-make` en tu PATH).
2. Si pusiste la carpeta `SFML` dentro del proyecto como se indicó arriba, simplemente ejecuta:
   ```bash
   make
   ```
   *(En Windows con MinGW puede que necesites escribir `mingw32-make` en lugar de `make`)*
3. **Importante (Solo Windows):** Antes de ejecutar, debes copiar los archivos `.dll` que están en la carpeta `SFML/bin` al lado del archivo `.exe` generado.
   ```powershell
   Copy-Item "SFML\bin\*.dll" -Destination "."
   ```
4. Ejecuta el programa:
   ```bash
   ./spreadsheet_app
   ```
## Ejecución

Ejecuta el archivo generado:
```bash
./spreadsheet_app
```

## Uso de la Interfaz

*   **Navegación:** Usa las **flechas del teclado** para moverte por las celdas. La celda seleccionada se resaltará en azul. La vista hará "scroll" automáticamente si llegas al borde.
*   **Edición:** Presiona **Enter** sobre una celda para editarla (el borde se pondrá rojo). Escribe texto, números o fórmulas (ej. `=A1+2`). Presiona **Enter** para confirmar o **Esc** para cancelar.
*   **Eliminación de Celda:** Presiona **Delete** (Supr) sobre una celda, o deja el texto vacío al editar y presiona Enter, para borrarla completamente de la memoria.
*   **Fórmulas Aritméticas:** Cualquier celda que empiece con `=` se evaluará. Soporta `+`, `-`, `*`, `/`, paréntesis y referencias a otras celdas (ej. `=A1*(B2+C3)`).

### Comandos Avanzados (Barra Inferior)
Para ejecutar operaciones de rango, filas, columnas o navegación, entra en modo edición (Enter) en *cualquier* celda y escribe un comando empezando con `/`:

**Operaciones de Rango:** (Muestran el resultado en la barra inferior **y lo escriben estáticamente en la celda actual**)
*   `/SUMA A1 C3` -> Suma el rango.
*   `/PROM A1 C3` -> Calcula el promedio del rango.
*   `/MAX A1 C3` -> Encuentra el valor máximo en el rango.
*   `/MIN A1 C3` -> Encuentra el valor mínimo en el rango.

**Operaciones de Filas/Columnas Completas:** (Muestran el resultado y lo escriben en la celda actual)
*   `/SUMROW 5` -> Suma todos los números de la Fila 5.
*   `/SUMCOL B` -> Suma todos los números de la Columna B.

**Operaciones Estructurales (Eliminación):**
*   `/DELRANGE A1 C3` -> Elimina de memoria todas las celdas en el rectángulo.
*   `/DELROW 5` -> Elimina toda la fila 5 y reconecta los punteros verticales.
*   `/DELCOL B` -> Elimina toda la columna B y reconecta los punteros horizontales.

**Navegación Rápida:**
*   `/GOTO Z500` -> Desplaza la cámara instantáneamente a la celda especificada sin tener que mantener presionadas las flechas.
