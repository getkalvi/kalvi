'use strict';

module.exports = function(grunt) {

   var cwd = process.cwd();

  grunt.initConfig({
    // Watch for changes and trigger compass and livereload
    watch: {
      options: {
        cliArgs: ['--gruntfile', require('path').join(cwd, 'Gruntfile.js')]
      },
      compass: {
        files: ['scss/{,**/}*.scss'],
        tasks: ['compass:dev']
      },
      livereload: {
        options: {
          livereload: true
        },
        files: [
          'css/style.css',
          'js/*.js',
          'images/{,**/}*.{png,jpg,jpeg,gif,webp,svg}'
        ]
      }
    },

    // Compass and scss
    compass: {
      options: {
        cssDir: 'css',
        sassDir: 'scss',
        imagesDir: 'images',
        javascriptsDir: 'js',
        fontsDir: 'css/fonts',
        assetCacheBuster: 'none',
        require: [
	  'compass-normalize',
          'sass-globbing',
          'susy',
          'breakpoint'
        ]
      },
	dev: {
        options: {
          environment: 'development',
          outputStyle: 'expanded',
          relativeAssets: true,
          raw: 'line_numbers = :true\n'
        }
      },
      dist: {
        options: {
          environment: 'production',
          outputStyle: 'compressed',
          force: true
        }
      }
    }

});

  // Load the plugins.
  grunt.loadNpmTasks('grunt-contrib-compass');
  grunt.loadNpmTasks('grunt-contrib-watch');


  grunt.registerTask('default', [
    'compass:dev',
    'watch'
  ]);
  grunt.file.setBase('../');
};
