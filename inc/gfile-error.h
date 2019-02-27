#ifndef GFILE_ERROR_H
#define GFILE_ERROR_H

#define allocate(expr) if (!(expr)) goto gfile_error_catch;
#define throw(err) errno = err; goto gfile_error_catch;
#define raise() goto gfile_error_catch;
#define catch() gfile_error_catch:

#endif
