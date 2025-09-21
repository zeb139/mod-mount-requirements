[English](README.md) | [Español](README_ES.md)

## Descripción
Este módulo proporciona un sistema para personalizar los requisitos de montura en AzerothCore.  
Te permite realizar de forma segura las siguientes acciones:

* Modificar los costos de entrenamiento de la habilidad de montar y los requisitos de nivel (incluido el Tomo de vuelo en clima frío).
* Modificar los precios de compra/venta y los requisitos de nivel para las monturas.  
* Volver fácilmente a los valores predeterminados.
* Configuración independiente para las monturas de las clases paladín, brujo, druida y caballero de la Muerte.

## Requisitos
Se recomienda una versión reciente de AzerothCore. Este módulo utiliza identificadores de objetos y hechizos codificados de forma fija  
que espera encontrar en las tablas `acore_world.item_template` y `acore_world.npc_trainer`.

## Instalación
1. Descarga mod-mount-requirements en el directorio `modules` de tu fuente AzerothCore. 
2. Copia el archivo `mod_mount_requirements.conf.dist` de la carpeta `conf` en la carpeta de configuración del módulo de tu fuente AzerothCore. (`../path/to/azerothcore-wotlk/env/dist/etc/modules`). **Cambie el nombre del archivo** a `mod_mount_requirements.conf` (elimine la extensión `.dist`).
3. Vuelva a ejecutar cmake e inicie una compilación limpia de AzerothCore. En Linux, puede ejecutar `../path/to/azerothcore-wotlk/acore.sh compiler build` . (Consulte la documentación oficial de AzerothCore para obtener más información: https://www.azerothcore.org/wiki/installing-a-module).

## Uso
**Nota: Todos los usuarios de tu servidor deberán eliminar la carpeta «Cache» de su juego entre cada actualización del archivo .conf.** Si no lo hacen, los jugadores verán datos antiguos en las descripciones emergentes y es posible que no puedan «aprender» nuevas monturas, incluso si cumplen los requisitos que hayas configurado. Esto se debe a que el cliente del juego utiliza la información almacenada en caché para algunas de sus comprobaciones internas.  

1. Detén tu servidor si ya está en funcionamiento.  
2. Configura el archivo `mod_mount_requirements.conf` según sea necesario.  
3. Asegúrate de que Wow.exe no se esté ejecutando y, a continuación, elimina la carpeta `Cache` del mismo directorio que Wow.exe (esta se vuelve a crear cada vez que se ejecuta el juego).  
4. Inicia el servidor e inicia sesión.  

### Para revertir los cambios realizados por el módulo  
1. Siga los mismos pasos que arriba, pero primero establezca `MountRequirements.Enable = false` en su archivo `mod_mount_requirements.conf`.  
2. La próxima vez que se ejecute el servidor, se aplicarán todos los requisitos de montaje originales a la base de datos. (*Recuerde eliminar su carpeta `Cache`*)