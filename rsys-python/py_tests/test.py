import rsys

if __name__ == "__main__":
    m = rsys.math.matrix(4, 4)
    m.set(0, 0, 4)
    m.set(0, 1, 5)
    m.set(0, 2, 2)
    m.set(0, 3, -1)

    m.set(1, 0, -1)
    m.set(1, 1, 4)
    m.set(1, 2, 4)
    m.set(1, 3, 3)

    m.set(2, 0, -1)
    m.set(2, 1, 2)
    m.set(2, 2, -1)
    m.set(2, 3, -1)

    m.set(3, 0, 5)
    m.set(3, 1, 4)
    m.set(3, 2, -1)
    m.set(3, 3, -1)

    svd = rsys.SVD(m, 6)
    svd.learn(0.1, 0.5, 1000)

    for i in xrange(0, m.rows):
        for j in xrange(0, m.cols):
            print "%d " % m.at(i, j),
        print
    print

    for i in xrange(0, m.rows):
        for j in xrange(0, m.cols):
            print "%f " % svd.recommendation(i, j),
        print