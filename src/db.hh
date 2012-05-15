#ifndef DB_HH
# define DB_HH

# include <sqlite3.h>
# include <mimosa/singleton.hh>

class Db : public mimosa::Singleton<Db>
{
public:
  Db();
  ~Db();

  static sqlite3 * handle();

private:
  sqlite3 * db_;
};

#endif /* !DB_HH */
