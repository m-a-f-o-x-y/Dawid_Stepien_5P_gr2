using System.Xml.Serialization;
using static Program;

namespace testySolvera
{
    public class UnitTest1
    {
        [Fact]
        public void zerowaFunkcjaLiniowa()
        {
            bool error = false;
            try
            {
                LinearFunction l1 = new LinearFunction(0, 0);
            }
            catch (Exception)
            {
                error = true;
            }

            Assert.True(error); 
            
        }

        [Fact]
        public void wynikNiewymierny()
        {
            LinearFunction l1 = new LinearFunction(MathF.E, -4);
            LinearFunction l2 = new LinearFunction(MathF.PI / 6, -MathF.Sqrt(2));

            float? result = l1.Intersection(l2);

            Assert.Equal(result, 1.1782049504854f);
        }

        [Fact]
        public void jednoRozwFnKwadratowej()
        {

        }

    }
}
