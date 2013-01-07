mergeInto(LibraryManager.library, {
    save_config_js: function(){
        var documentsDir;
        var ConfigFile;
        Module.print(' Save Config JS ');

      //  Module.print(String.fromCharCode.apply(String, FS.analyzePath('config.xml').object.contents));
        try {
            tizen.filesystem.resolve('wgt-private',
                         function(dir){
                            documentsDir = dir;
                            Module.print('\'wgt-private\' resolved ' + documentsDir);

                            try {
                                Module.print("Dir path " + dir.path);
                                ConfigFile = dir.resolve('config.xml');
                                Module.print('Resolve file config.xml ');
                                try {

                                    ConfigFile.openStream(
                                        'w', function(filetostream){
                                              filetostream.write(String.fromCharCode.apply(String, FS.analyzePath('config.xml').object.contents));
                                            filetostream.close();
                                         }, function(e){
                                            Module.print("Error" + e.message);
                                         }
                                      );
                                } catch (exc){
                                    Module.print('Write() exception:' + exc.message);
                                }
                            } catch (exc){
                                Module.print('Not resolve file config.xml ' + exc.message + '<br/>');
                            }
                         },
                         function(e){
                            Module.print("Error" + e.message);
                         } ,
                         'rw');
        } catch (exc){
              Module.print("Error" + exc.message);
        }
    },
    prepare_config_js: function(){

        var documentsDir;
        var ConfigFile;
        var ConfigFileFS;

        //dirname = Pointer_stringify(directory);

        var prepareconfig = Module.cwrap('prepareconfig', null, []);
        Module.print(' Try to resolve  directory ' + documentsDir);
        try {
            tizen.filesystem.resolve('wgt-private',
                         function(dir){
                            documentsDir = dir;
                            Module.print('\'wgt-private\' resolved ' + documentsDir);

                            try {
                                Module.print("Dir path " + dir.path);
                                ConfigFile = dir.resolve('config.xml');
                                Module.print('Resolve file config.xml ');
                                try {
                                    ConfigFile.readAsText(
                                        function(contents) {
                                           ConfigFileFS = FS.createDataFile('/', 'config.xml', contents, true, true);
                                            Module.print('Config file config.xml is done');
                                            prepareconfig(1);
                                        },
                                        function(e){
                                            Module.print("Error" + e.message);
                                        }
                                    );
                                } catch (exc){
                                    Module.print('readAsText() exception:' + exc.message);
                                }
                            } catch (exc){
                                Module.print('Not resolve file config.xml ' + exc.message + '<br/>');
                                try {
                                    ConfigFile = dir.createFile('config.xml');
                                    Module.print('Create file config.xml ');
                                    ConfigFileFS = FS.createDataFile('/', 'config.xml', '', true, true);
                                    prepareconfig(1);
                                } catch (exc){
                                    Module.print('Not create file config.xml ' + exc.message + '<br/>');
                                }
                            }
                        },
                         function(e){
                            Module.print("Error" + e.message);
                            prepareconfig(1);
                         } ,
                         'rw');
         } catch (exc) {
            Module.print('tizen.filesystem.resolve() exception: ' + exc.message + '<br/>');
         } 
    }
});

