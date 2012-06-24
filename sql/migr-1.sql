begin transaction;

alter table paste add column parent_past_id integer null default null;
alter table paste add column content_type text null default null;

update paste set content_type = 'text/plain';
pragma schema_version = 2;

commit transaction;
