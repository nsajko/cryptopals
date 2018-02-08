// Copyright 2018 (c), Neven Sajko. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the
// LICENSE file.

package test

import "testing"

func TestGeneralization(t *testing.T) {
	for b1 := 0x00; b1 <= 0xff; b1++ {
		for b2 := 0x00; b2 <= 0xff; b2++ {
			concatSlice := []byte{byte(b1), byte(b2)}
			if float64(hammingDistance([]byte{byte(b1)}, []byte{byte(b2)})) != generalizedHammingDistance(concatSlice, 1) {
				t.Fail()
				t.Logf("Supposed generalization not equal to Hamming distance for %2x %2x\n", b1, b2)
			}
		}
	}
}
