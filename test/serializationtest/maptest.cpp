#include <iostream>
#include <map>
#include <fstream>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <string>
using namespace std;

class Coolmap {
  public:
    friend class boost::serialization::access;
    std::map<string, string> nicemap;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& nicemap;
    }
    vertexVector Acopy;
    save() const
    {
        std::ofstream outfile(".Simplex.save");
        boost::archive::text_oarchive oa(outfile);
        oa << Acopy << AdditionalInfortmation;
    }

    restore()
    {
        std::ifstream infile(".Simplex.save");
        boost::archive::text_iarchive ia(infile);
        ia >> Acopy >> AdditionalInformation;
    }
};

int main()
{
    // create a Map and insert into it
    Coolmap A;
    A.nicemap.insert(make_pair("coolmap1", "ist cool"));
    A.nicemap.insert(make_pair("coolmap2", "ist nicht cool"));

    // IMPORTANT::must be inside a scope(brackets); Writing the object to the archive (and file )
    {
        std::ofstream outfile("test123");
        boost::archive::text_oarchive oa(outfile);
        oa << A;
    }

    // IMPORTANT::must be inside a scope(brackets);Creating an object and read in from that exact
    // file to the object)
    Coolmap B;
    {
        std::ifstream infile("test123");
        boost::archive::text_iarchive ia(infile);
        ia >> B;
    }
    // check if the contents of B are the same as in A
    for (auto iter : B.nicemap) {
        std::cout << iter.first << " " << iter.second << std::endl;
    }

    return 0;
}
