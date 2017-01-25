const int kFirstPin = 5;
const int kNumCells = 8;

byte cells[kNumCells];
// Wolfram's rule 110:
// (x1 x2 x3) -> new x2
// 000 -> 0
// 001 -> 1
// 010 -> 1
// 011 -> 1
// 100 -> 0
// 101 -> 1
// 110 -> 1
// 111 -> 0
// It is equal to !x1 & x3 v x2^x3

byte apply_rule_110(int left, int middle, int right) {
  return (~cells[left]) & cells[right] | (cells[middle] ^ cells[right]);
}

void setup() {
 randomSeed(analogRead(0));
 for (int i = 0; i < kNumCells; ++i) {
   pinMode(kFirstPin + i, OUTPUT);
   cells[i] = random(0, 2);
   cells[0] = 1;
   digitalWrite(kFirstPin + i, cells[i]);
 }
 delay(500);
}

void loop() {
 // Automaton in cycled: last cell is neighbor to first cell.
 byte new_cells[kNumCells];

 new_cells[0] = apply_rule_110(kNumCells - 1, 0, 1);
 new_cells[kNumCells - 1] = apply_rule_110(kNumCells - 2, kNumCells - 1, 0);
 digitalWrite(kFirstPin, new_cells[0]);
 digitalWrite(kFirstPin + kNumCells - 1, new_cells[kNumCells - 1]);

 for (int i = 1; i < kNumCells - 1; ++i) {
   new_cells[i] = apply_rule_110(i - 1, i, i + 1);
   digitalWrite(kFirstPin + i, new_cells[i]);
 }
 memcpy(cells, new_cells, sizeof(byte) * kNumCells);
 delay(500);
}
