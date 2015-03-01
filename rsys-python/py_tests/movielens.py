import rsys
import os
import random
import math
import pylab


def parse_data():
    training_set = []
    test_set = []

    prefix = '/home/igor/Projects/cpp/rsys/datasets/ml-1m/'
    with open(os.path.join(prefix, 'ratings.dat')) as f:
        for line in f:
            d = line.split('::')

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
    with open("features_train_set.dat", 'w') as train_set_file:
        with open("features_test_set.dat", 'w') as test_set_file:
            for d in xrange(1, 11):
                config = rsys.SVDConfig(users_count, items_count, -1, d, 0.005)
                config.set_print_result(False)
                config.assign_seq_ids()
                config.set_max_iterations(500)
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
    l = 0.0001
    with open("lambda_train_set.dat", 'w') as train_set_file:
        with open("lambda_test_set.dat", 'w') as test_set_file:
            while l < 1.5:
                config = rsys.SVDConfig(users_count, items_count, -1, 4, l)
                config.set_print_result(False)
                config.assign_seq_ids()
                config.set_max_iterations(500)
                svd = rsys.SVD(config)

                training_set, test_set = parse_data()
                training_rmse = svd.learn_offline(training_set)

                test_rmse = count_rmse(svd, test_set)
                print 'l = %f. trainging_set RMSE = %f. test_set RMSE = %f' % (l, training_rmse, test_rmse)
                train_set_error.append((l, training_rmse))
                test_set_error.append((l, test_rmse))

                train_set_file.write("%f:%f:\n" % (l, training_rmse))
                test_set_file.write("%f:%f:\n" % (l, test_rmse))

                l *= 3

    return train_set_error, test_set_error


def main():
    users_count = 6040
    items_count = 3952

    est_features = False
    est_lambda = True

    if est_features:
        f_train_set_error, f_test_set_error = features(users_count, items_count)
        plot_data(f_train_set_error, f_test_set_error, 'features_count.png',
                  title='RMSE vs features count', xlabel='features count')

    if est_lambda:
        l_train_set_error, l_test_set_error = lambda_est(users_count, items_count)
        plot_data(l_train_set_error, l_test_set_error, 'lambda.png',
                  title='RMSE vs lambda', xlabel='lambda')


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print 'KeyboardInterrupt'
        exit()