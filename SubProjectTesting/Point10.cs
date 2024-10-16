using System.Runtime.InteropServices;
using System.Text;

namespace ConsoleApp3;

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct Point10
{
    public int X;
    public int Y;
    public int Z;
    public byte Intensity;
    public byte PackedByte;
    public byte Classification;
    public byte ScanAngleRank;
    public byte UserData;
    public ushort PointSourceId;

    public byte ReturnNumber => (byte)(PackedByte & 0b0000_0111);
    public byte NumberOfReturns => (byte)((PackedByte & 0b0011_1000) >> 3);
    public bool ScanDirectionFlag => (PackedByte & 0b0100_0000) != 0;
    public bool EdgeOfFlightLine => (PackedByte & 0b1000_0000) != 0;

    public override string ToString()
    {
        var sb = new StringBuilder();
        sb.AppendLine($"X: {X:X8}");
        sb.AppendLine($"Y: {Y}");
        sb.AppendLine($"Z: {Z:X8}");
        sb.AppendLine($"Intensity: {Intensity}");
        sb.AppendLine($"ReturnNumber: {ReturnNumber}");
        sb.AppendLine($"NumberOfReturns: {NumberOfReturns}");
        sb.AppendLine($"ScanDirectionFlag: {ScanDirectionFlag}");
        sb.AppendLine($"EdgeOfFlightLine: {EdgeOfFlightLine}");
        sb.AppendLine($"Classification: {Classification}");
        sb.AppendLine($"ScanAngleRank: {ScanAngleRank}");
        sb.AppendLine($"UserData: {UserData}");
        sb.AppendLine($"PointSourceId: {PointSourceId}");
        return sb.ToString();
    }
}