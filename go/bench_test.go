package c2c_bench

import (
	"sync"
	"sync/atomic"
	"testing"
)

type Unpadded struct {
	a uint64
	b uint64
}

type Padded struct {
	a    uint64
	_pad [56]byte
	b    uint64
	_pad2 [56]byte
}

func BenchmarkFalseSharing_Unpadded(b *testing.B) {
	var s Unpadded
	b.ResetTimer()
	b.RunParallel(func(pb *testing.PB) {
		for pb.Next() {
			atomic.AddUint64(&s.a, 1)
			atomic.AddUint64(&s.b, 1)
		}
	})
}

func BenchmarkFalseSharing_Padded(b *testing.B) {
	var s Padded
	b.ResetTimer()
	b.RunParallel(func(pb *testing.PB) {
		for pb.Next() {
			atomic.AddUint64(&s.a, 1)
			atomic.AddUint64(&s.b, 1)
		}
	})
}
