import rsys
import os
import random
import math
import pylab
from db import Db

db = Db('/home/igor/Projects/python/vkrsys/config.conf')
users = db.get_users_ids()
items = db.get_items_ids()

def parse_data():
    training_set = []
    test_set = []

    data = db.execute('SELECT user_id, song_id, rating FROM app_rating ORDER BY user_id, song_id')

    for d in data:
        score = rsys.ItemScore(int(d[0]), int(d[1]), int(d[2]))
        if random.random() < 0.67:
            training_set.append(score)
        else:
            test_set.append(score)
    return training_set, test_set

def count_rmse(svd, test_set):
    print 'Counting test_set RMSE'
    rmse = 1.0
    for s in test_set:
        predicted = svd.predict(s.user_id, s.item_id)
        actual = s.score
        e = predicted - actual
        rmse += e * e

    rmse /= len(test_set)
    rmse = math.sqrt(rmse)
    return rmse


def plot_data(train_set_data, test_set_data, filename, title, xlabel='x'):
    pylab.matplotlib.rc('font', family='Arial')
    pylab.clf()
    plots = [
        dict(points=train_set_data, label='Training set RMSE'),
        dict(points=test_set_data, label='Test set RMSE'),
    ]

    pylab.figure(0)
    pylab.title(title)
    pylab.xlabel(xlabel)
    pylab.ylabel('RMSE')
    pylab.grid(True)

    for p in plots:
        pylab.plot(*zip(*p['points']), label=p['label'])

    pylab.legend(loc='center right', fancybox=True, shadow=True, bbox_to_anchor=(1.05, 0.5))
    pylab.savefig(filename)


def features(users_count, items_count):
    train_set_error = []
    test_set_error = []


    mysql_conf = rsys.exporters.SVDMySQLConfig()
    mysql_conf.user = db.conn_info['user']
    mysql_conf.password = db.conn_info['passwd']
    mysql_conf.db_name = db.conn_info['db']
    mysql_conf.users_table = "auth_user"
    mysql_conf.items_table = "app_song"

    with open("vkrsys_features_train_set.dat", 'w') as train_set_file:
        with open("vkrsys_features_test_set.dat", 'w') as test_set_file:
            for d in xrange(1, 11):
                config = rsys.SVDConfig(users_count, items_count, -1, d, 0.05)
                config.set_print_result(False)
                config.set_users_ids(users)
                config.set_items_ids(items)
                config.set_predictor('sigmoid')
                # config.set_mysql_exporter(mysql_conf)
                svd = rsys.SVD(config)

                training_set, test_set = parse_data()
                training_rmse = svd.learn_offline(training_set)

                test_rmse = count_rmse(svd, test_set)
                print 'd = %d. trainging_set RMSE = %f. test_set RMSE = %f' % (d, training_rmse, test_rmse)
                train_set_error.append((d, training_rmse))
                test_set_error.append((d, test_rmse))

                train_set_file.write("%d:%f:\n" % (d, training_rmse))
                test_set_file.write("%d:%f:\n" % (d, test_rmse))

    return train_set_error, test_set_error


def lambda_est(users_count, items_count):
    train_set_error = []
    test_set_error = []


    mysql_conf = rsys.exporters.SVDMySQLConfig()
    mysql_conf.user = db.conn_info['user']
    mysql_conf.password = db.conn_info['passwd']
    mysql_conf.db_name = db.conn_info['db']
    mysql_conf.users_table = "auth_user"
    mysql_conf.items_table = "app_song"

    train_source = \
        rsys.ds.MySQLSource(mysql_conf,
                            "SELECT user_id, song_id, rating FROM app_rating ORDER BY user_id, song_id LIMIT 2000")
    valid_source = db.execute("SELECT user_id, song_id, rating FROM app_rating ORDER BY user_id, song_id LIMIT 10000 OFFSET 2000")

    l = 0.00001
    with open("vkrsys_lambda_train_set.dat", 'w') as train_set_file:
        with open("vkrsys_lambda_test_set.dat", 'w') as test_set_file:
            while l < 0.01:
                config = rsys.SVDConfig(users_count, items_count, -1, 6, l)
                config.set_print_result(False)
                config.set_users_ids(users)
                config.set_items_ids(items)
                config.set_predictor('sigmoid')
                # config.set_mysql_exporter(mysql_conf)
                svd = rsys.SVD(config)

                training_set, test_set = parse_data()
                training_rmse = svd.learn_offline(training_set)

                test_rmse = count_rmse(svd, test_set)
                print 'l = %f. trainging_set RMSE = %f. test_set RMSE = %f' % (l, training_rmse, test_rmse)
                train_set_error.append((l, training_rmse))
                test_set_error.append((l, test_rmse))

                train_set_file.write("%f:%f:\n" % (l, training_rmse))
                test_set_file.write("%f:%f:\n" % (l, test_rmse))

                l *= 2

    return train_set_error, test_set_error


def main():
    users_count = 6040
    items_count = 3952

    est_features = True
    est_lambda = True

    if est_features:
        f_train_set_error, f_test_set_error = features(users_count, items_count)
        plot_data(f_train_set_error, f_test_set_error, 'vkrsys_features_count.png',
                  title='RMSE vs features count', xlabel='features count')

    if est_lambda:
        l_train_set_error, l_test_set_error = lambda_est(users_count, items_count)
        plot_data(l_train_set_error, l_test_set_error, 'vkrsys_lambda.png',
                  title='RMSE vs lambda', xlabel='lambda')


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print 'KeyboardInterrupt'
        exit()
