add_executable(paste-binoused
  main.cc
  config.cc
  bottleneck.cc

  # web logic and common stuff
  db.cc
  load-tpl.cc
  page-footer.cc
  page-header.cc

  # handlers
  post-handler.cc
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
  pthread
  rt
)

install(TARGETS paste-binoused
  RUNTIME DESTINATION bin)
