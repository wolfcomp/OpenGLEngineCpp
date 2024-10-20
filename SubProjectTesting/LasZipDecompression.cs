using System;
using System.Collections.Generic;
using System.IO;

namespace ConsoleApp3;

public partial class LasZipDecompression
{
    private BinaryReader _reader;
    private int _base;
    private int _value;
    private int _length;


    public void InitializeFromStream(BinaryReader reader)
    {
        _value = reader.ReadByte() << 24 | reader.ReadByte() << 16 | reader.ReadByte() << 8 | reader.ReadByte();
        _length = unchecked((int)0xFFFFFFFF);
        _reader = reader;
    }

    public void InitializeDecoders()
    {

    }

    public void RenormDecInterval()
    {
        while (_length < 0x00FFFFFF)
        {
            _value = (_value & 0x00FFFFFF) << 8 | _reader.ReadByte();
            _length = (_length & 0x00FFFFFF) << 8;
        }
    }

    public ref struct SymbolDecoder(ref BinaryReader reader, ref int value, ref int length, Action renormFunc)
    {
        private readonly ref BinaryReader _reader = ref reader;
        private readonly ref int _value = ref value;
        private readonly ref int _length = ref length;
        private readonly Action _renormDecInterval = renormFunc;
        private ushort[] _symbolCount;
        private ushort[] _distribution;
        private ushort _updateCycle;
        private ushort _symbolsUntilUpdate;

        public void InitializeDecoder(int symbols)
        {
            _symbolCount = new ushort[symbols];
            _distribution = new ushort[symbols];
            for (var i = 0; i < symbols; i++)
            {
                _symbolCount[i] = 1;
            }
        }

        public void UpdateDistribution()
        {
            var symbols = _symbolCount.Length;
            var sum = 0;
            for (var i = 0; i < symbols - 1; i++)
            {
                sum += _symbolCount[i];
            }

            if (sum > 0x7FFF)
            {
                for (var i = 0; i < symbols - 1; i++)
                {
                    _symbolCount[i] = (ushort)((_symbolCount[i] + 1) / 2);
                }
            }
            sum = 0;
            for (var i = 0; i < symbols - 1; i++)
            {
                sum += _symbolCount[i];
            }
            var scale = 0x80000000 / sum;
            for (var i = 0; i < symbols - 1; i++)
            {
                _distribution[i] = (ushort)((scale * sum) >> 16);
                sum += _symbolCount[i];
            }
            _updateCycle = (ushort)((5 - _updateCycle) / 4);
            _updateCycle = Math.Min((ushort)(8 * (_symbolCount.Length + 6)), _updateCycle);
            _symbolsUntilUpdate = _updateCycle;
        }

        public int DecodeSymbol()
        {
            var l = _length >> 15;
            int symbol;
            for (symbol = 0; symbol < _symbolCount.Length - 1; symbol++)
            {
                if (_distribution[symbol] * l > _value)
                {
                    break;
                }
            }
            _value -= _distribution[symbol] * l;

            if (symbol < _symbolCount.Length - 1)
            {
                _length = _distribution[symbol + 1] * l - _distribution[symbol] * l;
            }
            else
            {
                _length -= _distribution[symbol] * l;
            }

            if (_length < 0x00FFFFFF)
            {
                _renormDecInterval();
            }
            _symbolCount[symbol] = (ushort)(unchecked(_symbolCount[symbol] + 1));
            _symbolsUntilUpdate--;
            if (_symbolsUntilUpdate == 0)
            {
                UpdateDistribution();
            }
            return symbol;
        }
    }

    public enum DecoderType
    {
        SymbolDecoder64,
        SymbolDecoder256,
        ShortDecoder,
        IntDecoder
    }
}
