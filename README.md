# Asteroides (SFML, C++)

Proyecto: juego estilo Asteroids sencillo usando SFML. Los archivos están en `src/` y los encabezados en `include/`.

Compilar (PowerShell / mingw):

```powershell
# desde la raíz del proyecto
make
# ejecutar
.\bin\Asteroids.exe
```

Notas:
- Coloca las texturas opcionales en `assets/imagenes/` y la fuente en `assets/tipografia/arial.ttf`.
- Si falta una textura o fuente, el juego dibujará formas básicas y seguirá funcionando en lugar de cerrarse.
- Controles: Flechas (izq/der) para rotar, Flecha arriba para empuje, Espacio para disparar, Esc para salir, R para reiniciar tras perder, M para volver al menú desde pantalla de fin de juego.

Proyecto: 3er parcial — Programación avanzada
