using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
namespace ConsoleApp3;

internal class Program
{
    private static unsafe void Main(string[] args)
    {
        var path = @"C:\Users\magnu\Downloads\eksport_959441_20241008\774\data\32-05-519-229-56-01.laz";
        using var fs = new FileStream(path, FileMode.Open, FileAccess.Read);
        using var br = new BinaryReader(fs);
        var header = ReadLasHeader(br);
        var records = new List<LasVariableLengthRecords>();
        for (var i = 0; i < header.NumberOfVariableLengthRecords; i++)
        {
            records.Add(ReadNextRecord(br));
        }

        var lazRecord = records.FirstOrDefault(t => t is { RecordId: 22204, Reserved: 0xAABB or 0 });

        var lazVLRData = new LazVariableLengthRecordData();
        fixed (byte* ptr = lazRecord.Data)
        {
            lazVLRData.Compressor = (LazVariableLengthRecordData.CompressorType)(*(ushort*)ptr);
            lazVLRData.Coder = *(ushort*)(ptr + 2);
            lazVLRData.VersionMajor = *(ptr + 4);
            lazVLRData.VersionMinor = *(ptr + 5);
            lazVLRData.VersionRevision = *(ushort*)(ptr + 6);
            lazVLRData.Options = *(uint*)(ptr + 8);
            lazVLRData.ChunkSize = *(uint*)(ptr + 12);
            lazVLRData.NumberOfSpecialEVLR = *(long*)(ptr + 16);
            lazVLRData.OffsetOfSPecialEVLR = *(long*)(ptr + 24);
            lazVLRData.NumberOfRecords = *(ushort*)(ptr + 32);
            lazVLRData.Data = new LazVariableLengthRecordData.LazItemRecord[lazVLRData.NumberOfRecords];
            var lazVLRItemDataPtr = (LazVariableLengthRecordData.LazItemRecord*)(ptr + 34);
            for (var i = 0; i < lazVLRData.NumberOfRecords; i++)
            {
                lazVLRData.Data[i] = lazVLRItemDataPtr[i];
            }
        }

        Console.WriteLine(lazVLRData);

        // Read first item
        fs.Seek(header.OffsetToPointData, SeekOrigin.Begin);

        var bytes = br.ReadBytes(sizeof(Point10));

        var pointHandle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
        var point = (Point10)Marshal.PtrToStructure(pointHandle.AddrOfPinnedObject(), typeof(Point10))!;
        pointHandle.Free();

        var gpsTime = br.ReadDouble();

        Console.WriteLine(point);
        Console.WriteLine($"GPS Time: {gpsTime}");
        Console.WriteLine($"Reader position: {fs.Position:X}");
    }

    public static LasHeader1_2 ReadLasHeader(string path)
    {
        using var fs = new FileStream(path, FileMode.Open, FileAccess.Read);
        using var br = new BinaryReader(fs);

        return ReadLasHeader(br);
    }

    public static LasHeader1_2 ReadLasHeader(BinaryReader reader)
    {
        var headerSize = Marshal.SizeOf<LasHeader1_2>();
        var headerBytes = reader.ReadBytes(headerSize);

        var headerHandle = GCHandle.Alloc(headerBytes, GCHandleType.Pinned);
        var header = (LasHeader1_2) Marshal.PtrToStructure(headerHandle.AddrOfPinnedObject(), typeof(LasHeader1_2))!;
        headerHandle.Free();

        return header;
    }

    public static unsafe LasVariableLengthRecords ReadNextRecord(BinaryReader reader)
    {
        var record = new LasVariableLengthRecords();
        var recordBytes = reader.ReadBytes(54);
        fixed (byte* ptr = recordBytes)
        {
            record.Reserved = *(ushort*)ptr;
            record.UserIdString = new string((sbyte*)(ptr + 2));
            record.RecordId = *(ushort*)(ptr + 18);
            record.RecordLengthAfterHeader = *(ushort*)(ptr + 20);
            record.DescriptionString = new string((sbyte*)(ptr + 22));
        }

        var dataBytes = reader.ReadBytes(record.RecordLengthAfterHeader);
        record.Data = dataBytes;
        return record;
    }
}

