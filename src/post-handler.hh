#ifndef POST_HANDLER_HH
# define POST_HANDLER_HH

# include <mimosa/http/handler.hh>

class PostHandler : public mimosa::http::Handler
{
public:
  virtual bool handle(mimosa::http::RequestReader & request,
                      mimosa::http::ResponseWriter & response) const;
};

#endif /* !POST_HANDLER_HH */
