var gulp        = require('gulp');
var browserSync = require('browser-sync').create();

gulp.task('browser-sync', function() {
    browserSync.init({
        server: {
            baseDir: "./"
        }
    });
    gulp.watch("src/css/.css").on('change', browserSync.reload);
});

gulp.task("default", gulp.series(["browser-sync"]));