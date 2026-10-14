const config = {
  testEnvironment: 'jest-environment-jsdom',
  "moduleNameMapper": {
      "\\.(css|sass|jpg|svg)$": 
      ["<rootDir>/src/components/SideBar/SideBar",
       "<rootDir>/src/"
      ],
    },
  };

export default config;