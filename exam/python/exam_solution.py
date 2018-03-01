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
            ##
            self._postcards = [line for line in fd]

        self._parse_postcards()

    @property
    def number_of_postcards(self):
        return len(self._postcards)

    def write_file(self):
        pst_list = self._get_postcards_list()
    
        with open(self._file, 'w') as f:
            for line in pst_list:
                f.write(line)

    def update_file(self):
        pst_list = self._get_postcards_list()

        with open(self._file, 'a') as f:
            for line in pst_list:
                f.write(line)

    def update_lists(self):
        self.readFile(self._file)
        self.parsePostcards()

    def get_postcards_by_date_range(self, date_range):
        start_date, end_date = date_range

        res = []
        for k in self._date:
            k_date = datetime.strptime(k, '%Y-%m-%d')

            res.extend([x for i,x in enumerate(self._postcards) if k_date >= start_date and k_date <= end_date and i in self._date[k]])

        return res

    def get_postcards_by_sender(self, sender):
        if sender in self._from:
            return [x for i, x in enumerate(self._postcards) if i in self._from[sender]]
        else:
            return []

    def get_postcards_by_receiver(self, receiver):
        if receiver in self._to:
            return [x for i, x in enumerate(self._postcards) if i in self._to[receiver]]
        else:
            return []

    def _get_postcards_list(self):
        l = []
        template_str = "date:{_date}; from:{_from}; to:{_to};\n"

        for i, _ in enumerate(self._postcards):
            date_str, = [k for k in self._date if i in self._date[k]]
            from_str, = [k for k in self._from if i in self._from[k]]
            to_str, = [k for k in self._to if i in self._to[k]]

            l.append(template_str.format(_date=date_str, _from=from_str, _to=to_str))

        return l


    """
        Wrappers to the functions to meet the requirements and allowing
        the methods above to comply with the PEP-8 coding style.
    """

    def parsePostcards(self, *args, **kwargs):
        return self._parse_postcards(*args, **kwargs)

    def readFile(self, *args, **kwargs):
        return self.read_file(*args, **kwargs)

    def getNumberOfPostcards(self):
        return self.number_of_postcards

    def writeFile(self):
        return self.write_file()

    def updateFile(self):
        return self.update_file()

    def updateLists(self):
        return self.update_lists()

    def getPostcardsByDateRange(self, *args, **kwargs):
        return self.get_postcards_by_date_range(*args, **kwargs)

    def getPostcardsBySender(self, *args, **kwargs):
        return self.get_postcards_by_sender(*args, **kwargs)

    def getPostcardsByReceiver(self, *args, **kwargs):
        return self.get_postcards_by_receiver(*args, **kwargs)

