set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++14")

include_directories(.)

set(HEADER_FILES
                 # rsys/recommender.h
                 rsys/ratings_data.h
                 rsys/item_score.h
                 rsys/models/model.h

                 rsys/models/svd.h
                 rsys/config/svd_config.h

                 rsys/models/rbm.h

                 rsys/models/cf/cf_base.h
                 rsys/models/cf/cf_config.h
                 rsys/models/cf/cfuu.h
                 rsys/models/cf/cfuu_config.h
                 rsys/models/cf/cfii.h
                 rsys/models/cf/cfii_config.h
                 rsys/models/cf/aggregators/aggr.h
                 rsys/models/cf/aggregators/aggr_avg.h
                 rsys/models/cf/aggregators/aggr_simple.h
                 rsys/models/cf/aggregators/aggr_simple_biased.h
                 rsys/models/cf/simil/similarity.h
                 rsys/models/cf/simil/simil_pearson.h
                 rsys/models/cf/simil/simil_cos.h

                 rsys/config/config.h

                 rsys/db_conf/mysql_config.h

                 rsys/ensemblers/ensembler.h

                 rsys/exporters/mysql_exporter.h
                 rsys/exporters/svd_exporter.h
                 rsys/exporters/svd_mysql_exporter.h
                 rsys/exporters/errors.h

                 rsys/data_sources/map_index.h
                 rsys/data_sources/data_source.h
                 rsys/data_sources/mvector.h
                 rsys/data_sources/imatrix.h
                 rsys/data_sources/matrix.h
                 rsys/data_sources/map_matrix.h
                 rsys/data_sources/map_mvector.h
                 rsys/data_sources/sparse_matrix.h
                 rsys/data_sources/mexception.h
                 rsys/data_sources/db_source.h
                 rsys/data_sources/mysql_source.h

                 rsys/util/base_iterator.h
                 rsys/util/traits.h
                 rsys/util/util.h

                 rsys/predictors/predictor.h
                 rsys/predictors/linear_predictor.h
                 rsys/predictors/sigmoid_predictor.h
                 )

set(SOURCE_FILES rsys/data_sources/mexception.cc
                 rsys/core.cc
                 rsys/util/util.cc

                 rsys/db_conf/mysql_config.cc

                 rsys/exporters/mysql_exporter.cc
                 rsys/exporters/svd_mysql_exporter.cc
                 )

add_library(rsys SHARED ${HEADER_FILES} ${SOURCE_FILES} )
target_link_libraries(rsys mysqlcppconn)
