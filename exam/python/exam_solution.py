from __future__ import print_function
from datetime import datetime

class PostcardList(object):

    def __init__(self, file_to_load=None):
        self._file = file_to_load
        self._postcards = None
        self._date = {}
        self._from = {}
        self._to = {}

    def _parse_postcards(self):
        """Extract from each postcard its date, from and sender fields."""
        for postcard_idx, postcard in enumerate(self._postcards):

            try:
                date_raw, from_raw, to_raw, _ = postcard.split(";")
                _, date_str = date_raw.split("date:", 1)
                _, from_str = from_raw.split("from:", 1)
                _, to_str = to_raw.split("to:", 1)
            except ValueError as e:
                raise ValueError("Postcard {} from file {} has an unrecognized format, it should "
                                 "be `date:YYYY-MM-DD; from:sender; to:recipient`.\n"
                                 "Specific error: {}".format(postcard_idx, self._file, e))

            if date_str not in self._date:
                self._date[date_str] = []
            if from_str not in self._from:
                self._from[from_str] = []
            if to_str not in self._to:
                self._to[to_str] = []

            self._date[date_str].append(postcard_idx)
            self._from[from_str].append(postcard_idx)
            self._to[to_str].append(postcard_idx)


    def read_file(self, file_to_load=None):
        """Using the file loaded when creating the class, read and parse the fields.

        To comply with the unittests provided, the filepath can also be set as the optional
        parameter ``file_to_load``. If no filepath is given, it's assumed that it has already been
        set at the object initialization.
        """

        if file_to_load:
            self._file = file_to_load

        with open(self._file) as fd:
            # Strip away the newline and the trailing semicolon after the `to` field.
            self._postcards = [line for line in fd]

        self._parse_postcards()

    @property
    def number_of_postcards(self):
        return len(self._postcards)

    def writeFile(self):
        pass

    def updateFile(self):
        pass

    def updateLists(self):
        self.readFile(self._file)
        self.parsePostcards()

    def parsePostcards(self, *args, **kwargs):
        return self._parse_postcards(*args, **kwargs)

    def readFile(self, *args, **kwargs):
        return self.read_file(*args, **kwargs)

    def getNumberOfPostcards(self):
        return self.number_of_postcards

    def getPostcardsByDateRange(self, date_range):
        start_date, end_date = date_range

        res = []
        for k in self._date:
            k_date = datetime.strptime(k, '%Y-%m-%d')

            res.extend([x for i,x in enumerate(self._postcards) if k_date >= start_date and k_date <= end_date and i in self._date[k]])

        return res

    def getPostcardsBySender(self, sender):
        if sender in self._from:
            return [x for i, x in enumerate(self._postcards) if i in self._from[sender]]
        else:
            return []

    def getPostcardsByReceiver(self, receiver):
        if receiver in self._to:
            return [x for i, x in enumerate(self._postcards) if i in self._to[receiver]]
        else:
            return []
