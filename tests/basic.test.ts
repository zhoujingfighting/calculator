import { expect } from 'chai';
const { calculate} = require('../build/Release/calculator.node')
describe("Test caculator", () => {
    it("simple tests", () => {
        expect(calculate("1+2+3")).eq(6);
        expect(calculate("1+2*3")).eq(7);
        expect(calculate("(1+2)*6+3")).eq(21);
        expect(calculate("(16/2)*6+3")).eq(51);
        expect(calculate("(1/2)*6+3")).eq(3);
    })
})