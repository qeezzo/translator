using System.IO;
using System.Text;

namespace translator.Models.Queries;

public class DatabaseConfigQuery : Query {
  public override QueryType Type => QueryType.DatabaseConfigQuery;
  public string User = "";
  public string Server = "";
  public string Password = "";

  public override byte[] InnerBytes() {
    var stream = new MemoryStream();
    var writer = new BinaryWriter(stream);

    var bytes_user = Encoding.Default.GetBytes(User);
    var bytes_server = Encoding.Default.GetBytes(Server);
    var bytes_password = Encoding.Default.GetBytes(Password);

    writer.Write(bytes_user.Length);
    writer.Write(bytes_user);
    writer.Write(bytes_server.Length);
    writer.Write(bytes_server);
    writer.Write(bytes_password.Length);
    writer.Write(bytes_password);

    return stream.ToArray();
  }
}
