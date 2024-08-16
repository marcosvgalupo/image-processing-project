## Directories
1. **src** - contains the main file base.c
2. **utils** - contains the imagelib library for reading and writing images in pgm-ascii format

## Instalação
1. For linux não isn't necessary install any additional software
2. For windows:
   * Instalar o visualizador de imagens pgm-ascii [IrfanView](https://www.irfanview.com/64bit.htm);
   * And do this:
     - System(Control panel) -> Advanced System Settings -> Environment variables -> PATH (add the path "c:\Program Files\IrfanView")

## Compile and Run
1. Compile imagelib:
```
$ cd utils
$ make
```

2. Compile and run the example
```
$ cd codigoBase
$ make
$ main imgs/<image_name>.pgm
```
