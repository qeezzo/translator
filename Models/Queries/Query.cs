using System.IO;

namespace translator.Models.Queries;

public enum QueryType {
  TranslatorQuery = 1,
  DatabaseConfigQuery = 2,
  History = 3,
}

public abstract class Query {
  public abstract QueryType Type { get; }

  public byte[] ToBytes() {
    var stream = new MemoryStream();
    var writer = new BinaryWriter(stream);

    writer.Write(((int)Type));
    writer.Write(InnerBytes());

    return stream.ToArray();
  }

  public abstract byte[] InnerBytes();
}
