mergeInto(LibraryManager.library, {
    my_js: function() {
        var documentsDir;
        function onsuccess(files) {
            Module.print("FileName is!!!!!!!!!!!! " ); // displays file name
            for(var i = 0; i < files.length; i++) {
               Module.print("File Name is " + files[i].name); // displays file name
            }
            var testFile = documentsDir.createFile("test.txt");

            if (testFile != null) {
                testFile.openStream(
                  "w",
                  function(fs){
                       fs.write("HelloWorld");
                       fs.close();
                  }, function(e){
                      Module.print("Error " + e.message);
                  },"UTF-8"
                  );
            }
        }


        function onerror(error) {
                Module.print("The error " + error.message + " occurred when listing the files in the selected folder");
        }

        Module.print('hello from lib!');
        FS.createDataFile('/', 'config.xml', 'config.xml', true, true);
        try {
            tizen.filesystem.resolve('documents',
                         function(dir){
                            documentsDir = dir;
                            Module.print('\'documents\' resolved ' + documentsDir);
                            dir.listFiles(onsuccess,onerror);
                         },
                         function(e){
                            Module.print("Error" + e.message);
                         } ,
                         'rw');
         } catch (exc) {
            Module.print('tizen.filesystem.resolve() exception: ' + exc.message + '<br/>');
         } 
         Module.print('\'documents\' resolved <br/>');
    }
});

