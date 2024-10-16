using System;
using System.Runtime.InteropServices;
using System.Text;

namespace ConsoleApp3;

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public unsafe struct LasVariableLengthRecords
{
    public ushort Reserved; // 2 bytes
    public fixed byte UserId[16]; // 16 bytes
    public ushort RecordId; // 2 bytes
    public ushort RecordLengthAfterHeader; // 2 bytes
    public fixed byte Description[32]; // 32 bytes

    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0)]
    public byte[] Data;

    public string UserIdString
    {
        get
        {
            fixed (byte* ptr = UserId)
                return new string((sbyte*)ptr);
        }
        set
        {
            var bytes = Encoding.ASCII.GetBytes(value);
            fixed (byte* ptr = UserId)
                Marshal.Copy(bytes, 0, (IntPtr)ptr, bytes.Length);
        }
    }

    public string DescriptionString
    {
        get
        {
            fixed (byte* ptr = Description)
                return new string((sbyte*)ptr);
        }
        set
        {
            var bytes = Encoding.ASCII.GetBytes(value);
            fixed (byte* ptr = Description)
                Marshal.Copy(bytes, 0, (IntPtr)ptr, bytes.Length);
        }
    }

    public override string ToString()
    {
        var sb = new StringBuilder();
        sb.AppendLine($"Reserved: {Reserved:X4}");
        sb.AppendLine($"UserId: {UserIdString}");
        sb.AppendLine($"RecordId: {RecordId:X4}");
        sb.AppendLine($"RecordLengthAfterHeader: {RecordLengthAfterHeader}");
        sb.AppendLine($"Description: {DescriptionString}");
        sb.AppendLine($"Data:\n\t{BitConverter.ToString(Data)}");
        return sb.ToString();
    }
}

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct LazVariableLengthRecordData
{
    public CompressorType Compressor; // 2 bytes
    public ushort Coder; // 2 bytes
    public byte VersionMajor; // 1 byte
    public byte VersionMinor; // 1 byte
    public ushort VersionRevision; // 2 bytes
    public uint Options; // 4 bytes
    public uint ChunkSize; // 4 bytes
    public long NumberOfSpecialEVLR; // 8 bytes
    public long OffsetOfSPecialEVLR; // 8 bytes
    public ushort NumberOfRecords; // 2 bytes

    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 0)]
    public LazItemRecord[] Data;

    public override string ToString()
    {
        var sb = new StringBuilder();
        sb.AppendLine($"Compressor: {Compressor:G}");
        sb.AppendLine($"Coder: {Coder:X4}");
        sb.AppendLine($"VersionMajor: {VersionMajor}");
        sb.AppendLine($"VersionMinor: {VersionMinor}");
        sb.AppendLine($"VersionRevision: {VersionRevision}");
        sb.AppendLine($"Options: {Options}");
        sb.AppendLine($"ChunkSize: {ChunkSize}");
        sb.AppendLine($"NumberOfSpecialEVLR: {NumberOfSpecialEVLR}");
        sb.AppendLine($"OffsetOfSPecialEVLR: {OffsetOfSPecialEVLR}");
        sb.AppendLine($"NumberOfRecords: {NumberOfRecords}");
        sb.AppendLine("Data:");
        for (var index = 0; index < NumberOfRecords; index++)
        {
            var item = Data[index];
            sb.AppendLine($"\tItemType: {item.ItemType:G}");
            sb.AppendLine($"\tItemLength: {item.ItemLength}");
            sb.AppendLine($"\tItemVersion: {item.ItemVersion}");
            sb.AppendLine("");
        }

        return sb.ToString();
    }

    public enum CompressorType : ushort
    {
        None = 0,
        PointWise = 1,
        PointWiseChunked = 2,
        LayeredChunked = 3
    }

    public struct LazItemRecord
    {
        public LazItemType ItemType;
        public ushort ItemLength;
        public ushort ItemVersion;

        public enum LazItemType : ushort
        {
            Byte = 0,
            Short = 1,
            Integer = 2,
            Long = 3,
            Float = 4,
            Double = 5,
            Point10 = 6,
            GPSTime11 = 7,
            RGB12 = 8,
            Wavepacket13 = 9,
            Point14 = 10,
            RGB14 = 11,
            RGBNIR14 = 12,
            Wavepacket14 = 13,
            Byte14 = 14,
        }

        public override string ToString()
        {
            var sb = new StringBuilder();
            sb.AppendLine($"ItemType: {ItemType:G}");
            sb.AppendLine($"ItemLength: {ItemLength}");
            sb.AppendLine($"ItemVersion: {ItemVersion:X4}");
            return sb.ToString();
        }
    }
}