add_executable(paste-binoused
  main.cc
  config.cc
  bottleneck.cc
  purge.cc

  # web logic and common stuff
  db.cc
  load-tpl.cc
  page-footer.cc
  page-header.cc

  # handlers
  paste-handler.cc
  view-handler.cc
  error-handler.cc
)

target_link_libraries(paste-binoused
  mimosa-tpl
  mimosa-http
  mimosa-sqlite
  mimosa
  gnutls
  sqlite3
  lzma
  pthread
  rt
)

install(TARGETS paste-binoused
  RUNTIME DESTINATION bin)
