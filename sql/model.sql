create table if not exists paste
(
  paste_id integer not null primary key autoincrement,
  content blob not null,
  create_ts datetime not null default 'now',
  ip text not null default '0.0.0.0',
  -- encoding type:
  -- 0: identify
  -- 1: gzip
  -- 2: lzma
  encoding integer not null default 0
);
