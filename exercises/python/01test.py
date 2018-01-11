import unittest

function_problem1 = lambda num: sum(n for n in range(num + 1) if (n % 3 == 0 or n % 5 == 0))

# class Problem1(unittest.TestCase):

#     def test_whatever(self,):
#         self.assertEqual(2, 3)


class Problem1(unittest.TestCase):

    def setUp(self):
        self.func = function_problem1

    def test_size1000(self):
        res = self.func(1000)
        self.assertEqual(res, 234168)

    def test_size10000(self):
        res = self.func(10000)
        self.assertEqual(res, 23341668)

    def test_size100000(self):
        res = self.func(100000)
        self.assertEqual(res, 2333416668)

unittest.main()
