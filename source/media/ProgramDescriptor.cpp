#include "media/ProgramDescriptor.hpp"

#include "tools/Util.hpp"
#include "tools/Console.hpp"
#include "tools/SerializationImpl.hpp"

using namespace std;
using namespace Media;

ProgramDescriptor::ProgramDescriptor()
    : _tag{}
    , _length()
    , _data()
{
}

bool ProgramDescriptor::Parse(Tools::BitBuffer & buffer)
{
    _tag = static_cast<ProgramDescriptorTag>(buffer.ReadBits<uint8_t>(8));
    _length = buffer.ReadBits<uint8_t>(8);
    _data.clear();
    for (size_t index = 0; index < _length; ++index)
        _data.push_back(buffer.ReadBits<uint8_t>(8));
    return true;
}

bool ProgramDescriptor::IsValid() const
{
    return (_tag != ProgramDescriptorTag::Reserved1) &&
           (_tag != ProgramDescriptorTag::Reserved2) &&
           (!(_tag >= ProgramDescriptorTag::ReservedFirst) &&
             (_tag <= ProgramDescriptorTag::ReservedLast));

}

void ProgramDescriptor::DisplayContents() const
{
    Tools::DefaultConsole() << endl << "ProgramDescriptor" << endl;
    Tools::DefaultConsole() << "Tag:          " << _tag << " (0x" << Tools::Serialize((uint8_t)_tag, 16) << ")" << endl;
    Tools::DefaultConsole() << "Data length   0x" << Tools::Serialize(_length, 16) << endl;
    Tools::DefaultConsole() << "Data" << endl;
    Tools::DumpBytes(_data.begin(), _data.end());
}
