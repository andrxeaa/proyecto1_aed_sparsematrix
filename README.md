# Proyecto 1: Hoja de Cálculo (Listas Enlazadas Cruzadas)

Este proyecto implementa una hoja de cálculo usando una Matriz Dispersa basada en listas enlazadas cruzadas en C++ puro. Incluye una interfaz gráfica sencilla utilizando SFML.

## Requisitos Previos

Para compilar este proyecto necesitas:
1.  **Compilador C++17** (ej. `g++` en Windows MSYS2/MinGW, o en Linux).
2.  **Librería SFML 2.5 o superior** instalada en tu sistema.

## Compilación

Abre una terminal en este directorio y ejecuta:

```bash
make
```

Esto generará un ejecutable llamado `spreadsheet_app` (o `spreadsheet_app.exe` en Windows).

### Si usas Windows y tienes SFML en otra ruta
Edita las líneas comentadas en el `Makefile` para apuntar a los directorios `include` y `lib` correctos de tu instalación de SFML, por ejemplo:
`CXXFLAGS += -I"C:\SFML\include"`
`LDFLAGS = -L"C:\SFML\lib" -lsfml-graphics -lsfml-window -lsfml-system`

## Ejecución

Ejecuta el archivo generado:
```bash
./spreadsheet_app
```

## Uso de la Interfaz

*   **Navegación:** Usa las **flechas del teclado** para moverte por las celdas. La celda seleccionada se resaltará en azul. La vista hará "scroll" automáticamente si llegas al borde.
*   **Edición:** Presiona **Enter** sobre una celda para editarla (el borde se pondrá rojo). Escribe texto, números o fórmulas (ej. `=A1+2`). Presiona **Enter** para confirmar o **Esc** para cancelar.
*   **Eliminación de Celda:** Presiona **Delete** (Supr) sobre una celda para borrarla.
*   **Fórmulas Aritméticas:** Cualquier celda que empiece con `=` se evaluará. Soporta `+`, `-`, `*`, `/`, paréntesis y referencias a otras celdas (ej. `=A1*(B2+C3)`).

### Comandos Avanzados (Barra Inferior)
Para ejecutar operaciones de rango y eliminación de estructuras completas, entra en modo edición (Enter) en *cualquier* celda y escribe un comando empezando con `/`:

*   `/SUMA A1 C3` -> Muestra la suma del rango en la barra inferior.
*   `/PROM A1 C3` -> Muestra el promedio.
*   `/MAX A1 C3` -> Muestra el máximo.
*   `/MIN A1 C3` -> Muestra el mínimo.
*   `/DELRANGE A1 C3` -> Elimina todas las celdas en el rectángulo especificado.
*   `/DELROW 5` -> Elimina toda la fila 5.
*   `/DELCOL B` -> Elimina toda la columna B.

*Nota:* Después de ejecutar un comando con Enter, este no se guardará en la celda, sino que se ejecutará como acción global.
