#ifndef VIEW_HANDLER_HH
# define VIEW_HANDLER_HH

# include <mimosa/http/handler.hh>

class ViewHandler : public mimosa::http::Handler
{
public:
  virtual bool handle(mimosa::http::RequestReader & request,
                      mimosa::http::ResponseWriter & response) const;
};

#endif /* !VIEW_HANDLER_HH */
