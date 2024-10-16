using System;
using System.Runtime.InteropServices;
using System.Text;

namespace ConsoleApp3;

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public unsafe struct LasHeader1_2
{
    public fixed byte FileSignature[4];
    public ushort SourceId;
    public ushort GlobalEncoding;
    public fixed byte GuidData[16];
    public byte VersionMajor;
    public byte VersionMinor;
    public fixed byte SystemId[32];
    public fixed byte GeneratingSoftware[32];
    public ushort FileCreationDayOfYear;
    public ushort FileCreationYear;
    public ushort HeaderSize;
    public uint OffsetToPointData;
    public uint NumberOfVariableLengthRecords;
    public byte PointDataFormatId;
    public ushort PointDataRecordLength;
    public uint NumberOfPointRecords;
    public fixed uint NumberOfPointsByReturn[5];
    public double XScaleFactor;
    public double YScaleFactor;
    public double ZScaleFactor;
    public double XOffset;
    public double YOffset;
    public double ZOffset;
    public double MaxX;
    public double MinX;
    public double MaxY;
    public double MinY;
    public double MaxZ;
    public double MinZ;

    public Guid Guid
    {
        get
        {
            fixed (byte* ptr = GuidData)
                return new Guid(new ReadOnlySpan<byte>(ptr, 16));
        }
    }

    public string SystemIdString
    {
        get
        {
            fixed (byte* ptr = SystemId)
                return Encoding.ASCII.GetString(ptr, 32);
        }
    }

    public string GeneratingSoftwareString
    {
        get
        {
            fixed (byte* ptr = GeneratingSoftware)
                return new string((sbyte*)ptr);
        }
    }

    public override string ToString()
    {
        fixed (byte* ptr = FileSignature)
        {
            var sb = new StringBuilder();
            sb.AppendLine("FileSignature: " + Encoding.ASCII.GetString(ptr, 4));
            sb.AppendLine("SourceId: " + SourceId);
            sb.AppendLine("GlobalEncoding: " + GlobalEncoding);
            sb.AppendLine("Guid: " + Guid);
            sb.AppendLine("VersionMajor: " + VersionMajor);
            sb.AppendLine("VersionMinor: " + VersionMinor);
            sb.AppendLine("SystemId: " + SystemIdString);
            sb.AppendLine("GeneratingSoftware: " + GeneratingSoftwareString);
            sb.AppendLine("FileCreationDayOfYear: " + FileCreationDayOfYear);
            sb.AppendLine("FileCreationYear: " + FileCreationYear);
            sb.AppendLine("HeaderSize: " + HeaderSize);
            sb.AppendLine("OffsetToPointData: " + OffsetToPointData);
            sb.AppendLine("NumberOfVariableLengthRecords: " + NumberOfVariableLengthRecords);
            sb.AppendLine("PointDataFormatId: " + PointDataFormatId);
            sb.AppendLine("PointDataRecordLength: " + PointDataRecordLength);
            sb.AppendLine("NumberOfPointRecords: " + NumberOfPointRecords);
            sb.Append("NumberOfPointsByReturn: [");
            for (var i = 0; i < 5; i++)
                sb.Append(NumberOfPointsByReturn[i] + ", ");
            sb.Remove(sb.Length - 2, 2);
            sb.AppendLine("]");
            sb.AppendLine("XScaleFactor: " + XScaleFactor);
            sb.AppendLine("YScaleFactor: " + YScaleFactor);
            sb.AppendLine("ZScaleFactor: " + ZScaleFactor);
            sb.AppendLine("XOffset: " + XOffset);
            sb.AppendLine("YOffset: " + YOffset);
            sb.AppendLine("ZOffset: " + ZOffset);
            sb.AppendLine("MaxX: " + MaxX);
            sb.AppendLine("MinX: " + MinX);
            sb.AppendLine("MaxY: " + MaxY);
            sb.AppendLine("MinY: " + MinY);
            sb.AppendLine("MaxZ: " + MaxZ);
            sb.AppendLine("MinZ: " + MinZ);
            return sb.ToString();
        }
    }
}