#ifndef PASTE_HANDLER_HH
# define PASTE_HANDLER_HH

# include <mimosa/http/handler.hh>

class PasteHandler : public mimosa::http::Handler
{
public:
  virtual bool handle(mimosa::http::RequestReader & request,
                      mimosa::http::ResponseWriter & response) const;
};

#endif /* !PASTE_HANDLER_HH */
