using System.IO;
using System.Text;

namespace translator.Models.Queries;

public class TranslatorQuery : Query {
  public override QueryType Type => QueryType.TranslatorQuery;
  public Engine engine;
  public Lang source;
  public Lang dest;
  public Lang ui;
  public char[] data = [];

  public override byte[] InnerBytes() {
    var stream = new MemoryStream();
    var writer = new BinaryWriter(stream);

    byte[] default_bytes = Encoding.Default.GetBytes(data);

    writer.Write((int)engine);
    writer.Write((int)source);
    writer.Write((int)dest);
    writer.Write((int)ui);
    writer.Write((long)default_bytes.Length); // must be long
    writer.Write(default_bytes);

    return stream.ToArray();
  }
}
