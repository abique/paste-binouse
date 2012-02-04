create table if not exists paste
(
  paste_id integer not null primary key autoincrement,
  content text not null,
  create_ts timestamp null default '0'
);
