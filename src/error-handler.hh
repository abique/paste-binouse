#ifndef ERROR_HANDLER_HH
# define ERROR_HANDLER_HH

# include <mimosa/http/handler.hh>

bool errorHandler(mimosa::http::ResponseWriter & response,
                  const std::string & message);

#endif /* !ERROR_HANDLER_HH */
