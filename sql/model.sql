begin transaction;

create table if not exists paste
(
  paste_id integer not null primary key autoincrement,
  content blob not null,
  create_ts datetime not null default 'now',
  -- poster's ip
  ip text not null,
  -- encoding type:
  -- 0: identify
  -- 1: gzip
  -- 2: lzma
  encoding integer not null default 0,
  -- used to remember parent when forking
  parent_past_id integer null default null,
  -- used for syntaxic coloration
  content_type text null default null
);

pragma user_version = 2;

commit transaction;
